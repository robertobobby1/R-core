#pragma once
#include "rcpch.h"
#include "Core/Core.h"

namespace RC {
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
                    GetConfigMap()[key] = value;
                }
            }
        }

        static inline std::string Get(std::string& key) { return GetConfigMap()[key]; }

        static inline std::map<std::string, std::string>& GetConfigMap() { return s_config; }

       public:
        static std::map<std::string, std::string> s_config;
    };
}  // namespace RC