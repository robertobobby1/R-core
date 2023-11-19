#pragma once
#include "rcpch.h"
#include "Core/Core.h"

namespace RC {

    struct ConfigKeyPairs {
        std::vector<std::string> validValues;
        std::string defaultValue;
    };

    static std::map<std::string, ConfigKeyPairs> s_configKeyMap = {
        {"log_info", {{"true", "false"}, "true"}},
        {"log_debug", {{"true", "false"}, "true"}},
        {"log_warn", {{"true", "false"}, "true"}},
        {"log_error", {{"true", "false"}, "true"}},
        {"log_critical", {{"true", "false"}, "true"}},
        {"file_system_root_path", {{"path", "any"}, ""}}
    };

    class Config {
       public:
        static void Load() {
            std::ifstream configFile("config");
            if (!configFile.is_open()) {
                RC_LOG_WARN("Config file was not found! Default configurations will be applied!");
            }

            std::string line;
            while (std::getline(configFile, line)) {
                // remove all whitespaces
                line = std::regex_replace(line, std::regex("^ +| +$|() +"), "$1");

                std::istringstream keyLine(line);
                std::string key;

                if (!std::getline(keyLine, key, '=')) {
                    continue;
                }

                // get value
                std::string value;
                if (std::getline(keyLine, value)) {
                    s_config[key] = value;
                }
            }
            ValidateAndFormat();
        }

        static void ValidateAndFormat() {
            for (auto& keyConfig : s_configKeyMap) {
                auto value = Get(keyConfig.first);
                bool valueIsValid = false;
                bool isAnyValidValue = false;

                // check all valid values in array
                for (auto& validValue : keyConfig.second.validValues) {
                    RC_IF((validValue == value), valueIsValid = true; break;);

                    // special key cases
                    RC_IF((validValue == "any"), isAnyValidValue = true);
                    RC_IF((validValue == "path"), FormatFilePath(keyConfig.first.c_str()));
                }
                // if any then not checking for empty either
                RC_IF(isAnyValidValue, break);
                RC_IF((!valueIsValid && value != ""), RC_LOG_WARN("[{0}] is not a valid value for [{1}] ", value, keyConfig.first));

                RC_IF((value == ""),
                      RC_LOG_WARN("[{0}] is empty...", keyConfig.first);
                      valueIsValid = false;
                );

                RC_IF((!valueIsValid),
                      RC_LOG_WARN("Skipping value and defaulting to [{0}]", keyConfig.second.defaultValue);
                      s_config[keyConfig.first] = keyConfig.second.defaultValue;
                )
            }
        };

        static void FormatFilePath(const char* key) {
            auto value = Get(key);
            // remove last character if it is "/" to keep consistent paths
            if (value.back() == *"/") {
                s_config[key] = value.substr(0, value.length() - 1);
            }
        }

        static inline std::string Get(const char* key) { return s_config[key]; }
        static inline std::string Get(std::string key) { return s_config[key]; }

        static inline std::map<std::string, std::string>& GetConfigMap() { return s_config; }

       public:
        static std::map<std::string, std::string> s_config;
    };
}  // namespace RC