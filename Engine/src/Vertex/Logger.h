#pragma once





#include "Core.h"
#include "spdlog/spdlog.h"


namespace Vertex
{
	class VERTEX_API Logger
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return  s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return  s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

#define VX_CORE_TRACE(...)    ::Vertex::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define VX_CORE_INFO(...)     ::Vertex::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define VX_CORE_WARN(...)     ::Vertex::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define VX_CORE_ERROR(...)    ::Vertex::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define VX_CORE_CRITICAL(...) ::Vertex::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define VX_TRACE(...)         ::Vertex::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define VX_INFO(...)          ::Vertex::Logger::GetClientLogger()->info(__VA_ARGS__)
#define VX_WARN(...)          ::Vertex::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define VX_ERROR(...)         ::Vertex::Logger::GetClientLogger()->error(__VA_ARGS__)
#define VX_CRITICAL(...)      ::Vertex::Logger::GetClientLogger()->critical(__VA_ARGS__)