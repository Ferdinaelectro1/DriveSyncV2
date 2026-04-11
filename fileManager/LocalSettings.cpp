#include "LocalSettings.h"
#include <fstream>

LocalSettings::LocalSettings()
{
    if(!std::filesystem::is_directory(LOCAL_SETTINGS_DIR)) {
       std::filesystem::create_directory(LOCAL_SETTINGS_DIR);
    }
    if(!std::filesystem::exists(LOCAL_SETTINGS_PATH)) {
        std::ofstream output(LOCAL_SETTINGS_PATH);
        output.close();
    } else {
        std::ifstream input(LOCAL_SETTINGS_PATH);
        nlohmann::json json;
        if(!std::filesystem::is_empty(LOCAL_SETTINGS_PATH)) {
            input >> json;
            for(auto& [key,value] : json.items()) {
                _localElementIdMap[key] = value;
            }
        }
        input.close();
    }
}

std::optional<std::string> LocalSettings::getElementId(const std::string &elementName) const
{
    return _localElementIdMap.find(elementName) != _localElementIdMap.end() 
    ? std::optional<std::string>(_localElementIdMap.at(elementName)) 
    : std::nullopt;
}

void LocalSettings::setId(const std::string &elementName, const std::string &elementId)
{
    _localElementIdMap[elementName] = elementId;
    save();
}

void LocalSettings::removeElement(const std::string &elementName)
{
    if(_localElementIdMap.find(elementName) != _localElementIdMap.end()) {
        _localElementIdMap.erase(elementName);
    }
    save();
}

void LocalSettings::save()
{
    std::ofstream output(LOCAL_SETTINGS_PATH);
    if(output) {
        nlohmann::json json = _localElementIdMap;
        output << json;
    }
    output.close();
}
