#pragma once

#include <spdlog/fmt/fmt.h>


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"


#include "Base.h"
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

template <>
struct fmt::formatter<glm::vec3> {
    // Presentation format: 'f' - fixed, 'e' - exponential, default: 'f'.
    char presentation = 'f';

    // Parses format specifications.
    constexpr auto parse(fmt::format_parse_context& ctx) {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'e')) presentation = *it++;

        if (it != end && *it != '}') throw fmt::format_error("invalid format");
        return it;
    }

    // Formats the glm::vec3 using the parsed format spec.
    template <typename FormatContext>
    auto format(const glm::vec3& vec, FormatContext& ctx) {
        return fmt::format_to(
            ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z
        );
    }
};

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