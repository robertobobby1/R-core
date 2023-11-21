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

    static void PrintCppVersion() {
        std::string s;
        if (__cplusplus == 202101L)
            s += "C++23";
        else if (__cplusplus == 202002L)
            s += "C++20";
        else if (__cplusplus == 201703L)
            s += "C++17";
        else if (__cplusplus == 201402L)
            s += "C++14";
        else if (__cplusplus == 201103L)
            s += "C++11";
        else if (__cplusplus == 199711L)
            s += "C++98";
        else {
            s += "Unknown c++ standard";
        }
        RC_LOG_DEBUG("Cpp version running: {0}", s);
    }

    static void inline PrintSystem() {
#ifdef RC_PLATFORM_WINDOWS
        RC_LOG_DEBUG("System Windows detected");
#elif defined(RC_PLATFORM_LINUX)
        RC_LOG_DEBUG("System Linux detected");
#elif defined(RC_PLATFORM_MACOS)
        RC_LOG_DEBUG("System MacOS detected");
#endif
    }
}  // namespace RC