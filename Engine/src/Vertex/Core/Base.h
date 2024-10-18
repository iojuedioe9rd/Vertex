#pragma once

#include "Vertex/Core/PlatformDetection.h"
#include <memory>
#include "glm/glm.hpp"

#define BIT(x) (1 << x)
#define NULL 0
#ifndef VX_DIST
#define VX_ENABLE_ASSERTS 1
#endif // !VX_DIST

#ifdef VX_ENABLE_ASSERTS
	#if defined(VX_PLATFORM_WINDOWS)
		#define VX_DEBUGBREAK() __debugbreak()
	#elif defined(VX_PLATFORM_LINUX)
		#include <signal.h>
		#define VX_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define VX_ENABLE_ASSERTS
#else
	#define VX_DEBUGBREAK()
#endif



#define VX_EXPAND_MACRO(x) x
#define VX_STRINGIFY_MACRO(x) #x

#define VX_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Vertex {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
#include "Vertex/Core/Logger.h"
#include "Vertex/Core/Assert.h"