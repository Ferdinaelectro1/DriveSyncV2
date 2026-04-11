#include "LocalSettings.h"
#include <fstream>

LocalSettings::LocalSettings()
{
    const char* home = getenv("HOME");
    if(!home) {
        home = "/tmp"; 
    }
    _settingsDir = std::string(home) + "/.config/drivesync/";
    _settingsPath = std::string(home) + "/.config/drivesync/elementId.json";
    if(!std::filesystem::is_directory(_settingsDir)) {
       std::filesystem::create_directory(_settingsDir);
    }
    if(!std::filesystem::exists(_settingsPath)) {
        std::ofstream output(_settingsPath);
        output.close();
    } else {
        std::ifstream input(_settingsPath);
        nlohmann::json json;
        if(!std::filesystem::is_empty(_settingsPath)) {
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
    std::ofstream output(_settingsPath);
    if(output) {
        nlohmann::json json = _localElementIdMap;
        output << json;
    }
    output.close();
}
