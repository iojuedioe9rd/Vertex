#include "vxpch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Vertex
{
	Ref<spdlog::logger> Logger::s_CoreLogger;
	Ref<spdlog::logger> Logger::s_ClientLogger;
	void Logger::Init()
	{
		VX_PROFILE_FUNCTION();
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("VERTEX");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
		
	}
}