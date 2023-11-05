#include "rcpch.h"
#include "Core/Utils/Log.h"


namespace RC {

	std::shared_ptr<spdlog::logger> Log::s_logger;

	void Log::Init()
	{
		s_logger = spdlog::stdout_color_mt("general");
		s_logger->set_level(spdlog::level::trace);
		spdlog::set_pattern("[%H:%M:%S] [%^%v%$]");
	}
}