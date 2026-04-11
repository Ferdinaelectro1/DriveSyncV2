#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>

#define LOCAL_SETTINGS_DIR  "~/.config/drivesync/"
#define LOCAL_SETTINGS_PATH "~/.config/drivesync/elementId.json"

class LocalSettings {
    public:
      LocalSettings();
      [[nodiscard]] std::optional<std::string> getElementId(const std::string& elementName) const;
      void setId(const std::string& elementName, const std::string& elementId);
      void removeElement(const std::string& elementName);
    private:
      std::unordered_map<std::string,std::string> _localElementIdMap;
      void save();
};