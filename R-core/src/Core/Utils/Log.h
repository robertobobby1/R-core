#pragma once

#include "rcpch.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define RC_THREAD_ID() (std::hash<std::thread::id>{}(std::this_thread::get_id()))

typedef std::function<void(const spdlog::details::log_msg &msg)> LogCallbackFunction;

namespace RC {

    class Log {
       public:
        static void Init(LogCallbackFunction func = nullptr);

        static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }
        static std::shared_ptr<spdlog::pattern_formatter> GetLoggerFormatter() {
            return s_formatter;
        }

       private:
        static std::shared_ptr<spdlog::logger> s_logger;
        static std::shared_ptr<spdlog::pattern_formatter> s_formatter;
    };

    class LogFormatter : public spdlog::custom_flag_formatter {
       public:
        void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override {
            std::string serviceName = m_threadIdToServiceName[RC_THREAD_ID()];
            dest.append(serviceName.data(), serviceName.data() + serviceName.size());
        }

        std::unique_ptr<custom_flag_formatter> clone() const override {
            return spdlog::details::make_unique<LogFormatter>();
        }

        static void AddThreadName(size_t threadId, const char *serviceName) {
            m_threadIdToServiceName[threadId] = serviceName;
        };
        static std::map<size_t, std::string> m_threadIdToServiceName;
    };
}  // namespace RC
