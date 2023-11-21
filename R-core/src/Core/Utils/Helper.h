#pragma once
#include "rcpch.h"
#include "Core/Core.h"
#include "Core/Utils/Config.h"

#include "httplib.h"

namespace RC {

    template <typename T>
    std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    static inline void PrintConfig() {
        auto& configMap = Config::GetConfigMap();
        std::string s = "Configuration:\n";

        for (auto configPair : configMap) {
            s += fmt::format("{} = {}\n", configPair.first, configPair.second);
        }

        RC_LOG_DEBUG(s);
    }
}  // namespace RC