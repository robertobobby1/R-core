#include "rcpch.h"
#include "Core/Utils/Log.h"
#include "spdlog/pattern_formatter.h"

namespace RC {

    std::shared_ptr<spdlog::logger> Log::s_logger;

    void Log::Init() {
        auto formatter = std::make_unique<spdlog::pattern_formatter>();
        formatter->add_flag<LogFormatter>('*').set_pattern("[%H:%M:%S][%*]  [%^%v%$]");
        spdlog::set_formatter(std::move(formatter));

        s_logger = spdlog::stdout_color_mt("general");
        s_logger->set_level(spdlog::level::trace);
    }
}  // namespace RC