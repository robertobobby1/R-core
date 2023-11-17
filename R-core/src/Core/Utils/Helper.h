#pragma once
#include "rcpch.h"
#include "Core/Core.h"
#include "Core/Utils/Config.h"

namespace RC {

    template <typename T>
    std::string to_string(T value) {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    static inline void PrintConfig() {
        auto& configMap = Config::GetConfigMap();
        for (auto configPair : configMap) {
            RC_LOG_INFO("{0} = {1} ", configPair.first, configPair.second);
        }
    }
}  // namespace RC