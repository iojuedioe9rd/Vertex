#pragma once

#include "Vertex/PlatformDetection.h"
#include <memory>

#define BIT(x) (1 << x)
#define NULL 0
#ifndef VX_DIST

#endif // !VX_DIST


#ifdef VX_ENABLE_ASSERTS
#define VX_ASSERT(x, ...) { if(!(x)) { VX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define VX_CORE_ASSERT(x, ...) { if(!(x)) { VX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#include "Vertex/ErrorBox.h"

#define VX_ASSERT(x, ...) { if(!(x)) { ErrorBox("Assertion Failed"); __debugbreak(); exit(-1); } }
#define VX_CORE_ASSERT(x, ...) { if(!(x)) { ErrorBox("Core Assertion Failed"); __debugbreak(); exit(-1); } }
#endif

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