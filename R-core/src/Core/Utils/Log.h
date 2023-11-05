#pragma once

#include "rcpch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace RC {

	class Log {
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetLogger() { return s_logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};

}

