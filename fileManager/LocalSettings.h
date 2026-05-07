#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>

class LocalSettings {
    public:
      LocalSettings();
      [[nodiscard]] bool isloadingRequiredSettings();
      [[nodiscard]] std::optional<std::string> getElementId(const std::string& elementName) const;
      void setId(const std::string& elementName, const std::string& elementId);
      void removeElement(const std::string& elementName);
      [[nodiscard]] std::optional<std::string> getLocalConfig(const std::string& key);
      void setLocalConfig(const std::string& key, const std::string& value);
    private:
      std::unordered_map<std::string,std::string> _localElementIdMap;
      std::unordered_map<std::string,std::string> _localConfigMap;
      std::string _settingsDir;
      std::string _drive_id_settingsPath;
      std::string _config_settingsPath;
      void save();
};