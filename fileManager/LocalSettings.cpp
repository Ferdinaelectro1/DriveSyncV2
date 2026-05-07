#include "LocalSettings.h"
#include "../config/Config.h"
#include <fstream>

static void loadFromFile(const std::string &path, std::unordered_map<std::string, std::string> &map)
{
    if(!std::filesystem::exists(path)) {
        std::ofstream output(path);
        output.close();
    } else {
        std::ifstream input(path);
        nlohmann::json json;
        if(!std::filesystem::is_empty(path)) {
            input >> json;
            for(auto& [key,value] : json.items()) {
                map[key] = value;
            }
        }
        input.close();
    }
}

static void saveMap(const std::string &path, std::unordered_map<std::string, std::string> &map) {
    std::ofstream output1(path);
    if(output1) {
        nlohmann::json json = map;
        output1 << json;
    }
    output1.close();
}

LocalSettings::LocalSettings()
{
    const char* home = getenv("HOME");
    if(!home) {
        home = "/tmp"; 
    }
    _settingsDir = std::string(home) + "/.config/drivesync/";
    _drive_id_settingsPath = std::string(home) + "/.config/drivesync/elementId.json";
    _config_settingsPath = std::string(home) + "/.config/drivesync/config.json";
    if(!std::filesystem::is_directory(_settingsDir)) {
       std::filesystem::create_directory(_settingsDir);
    }
    loadFromFile(_drive_id_settingsPath,_localElementIdMap);
    loadFromFile(_config_settingsPath,_localConfigMap);
}

bool LocalSettings::isloadingRequiredSettings()
{
    if(_localConfigMap.find("client_id") == _localConfigMap.end())     return false;
    else if(_localConfigMap.find("client_secret") == _localConfigMap.end()) return false;
    else if(_localConfigMap.find("refresh_token") == _localConfigMap.end()) return false;
    return true;
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

std::optional<std::string> LocalSettings::getLocalConfig(const std::string &key)
{
    return _localConfigMap.find(key) != _localConfigMap.end() 
    ? std::optional<std::string>(_localConfigMap.at(key)) 
    : std::nullopt;
}

void LocalSettings::setLocalConfig(const std::string &key, const std::string &value)
{
    _localConfigMap[key] = value;
    save();
}

void LocalSettings::save()
{
    saveMap(_drive_id_settingsPath,_localElementIdMap);
    saveMap(_config_settingsPath,_localConfigMap);
}
