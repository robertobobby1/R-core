#include "rcpch.h"
#include "Core/Utils/Log.h"

#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/callback_sink.h"

namespace RC {

    std::shared_ptr<spdlog::logger> Log::s_logger;
    std::shared_ptr<spdlog::pattern_formatter> Log::s_formatter;

    void Log::Init(LogCallbackFunction func) {
        s_formatter = std::make_shared<spdlog::pattern_formatter>();
        s_formatter->add_flag<LogFormatter>('*').set_pattern("[%H:%M:%S][%*]  [%^%v%$]");
        auto formatter = s_formatter->clone();

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);

        std::vector<spdlog::sink_ptr> sinks({console_sink, file_sink});
        if (func != nullptr) {
            auto callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>(func);
            sinks.push_back(callback_sink);
        }
        s_logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());

        spdlog::register_logger(s_logger);
        spdlog::set_formatter(std::move(formatter));
        spdlog::set_level(spdlog::level::trace);
        spdlog::set_default_logger(spdlog::get("multi_sink"));
        spdlog::flush_every(std::chrono::seconds(5));
    }
}  // namespace RC