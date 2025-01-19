#pragma once

#include "Vertex/Core/PlatformDetection.h"
#include <memory>
#include "glm/glm.hpp"

#define BIT(x) (1 << x)
#define NULL 0
#ifndef VX_DIST
#define VX_ENABLE_ASSERTS 1
#endif // !VX_DIST

#define VX_STD_PATH_CSTR(path) (path.string().c_str())

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

#define VERTEX_BASE_STRUCT_BEGIN(name) namespace Vertex { struct VERTEX_API name { 
#define VERTEX_BASE_STRUCT_END(name) }; }

#define VERTEX_BASE_CLASS_BEGIN(name) namespace Vertex { class VERTEX_API name {
#define VERTEX_BASE_CLASS_END() }; }

#define VERTEX_EXTERN_C_START extern "C" {
#define VERTEX_EXTERN_C_END }

#define VERTEX_FOR_LOOP(name, count) for (int name = 0; name < count; name++)
#define VERTEX_CONST_PTR_TO_PTR(ptr, type) (type*)(void*)&(*ptr)

#define VERTEX_WHILE_LOOP(condition) while (condition)

#define VERTEX_WHILE_LOOP_MORE_OR_EQUAL_TO_ZERO(var) VERTEX_WHILE_LOOP(((var) >= 0))

#define VERTEX_IF_EQUAL(var, val) if (VERTEX_EQUAL(var, val))
#define VERTEX_IF_NOT_EQUAL(var, val) if (VERTEX_NOT_EQUAL(var, val))
#define VERTEX_EQUAL(var, val) (var VERTEX_EQUAL_SIGN val)
#define VERTEX_NOT_EQUAL(var, val) (!VERTEX_EQUAL(var, val))
#define VERTEX_EQUAL_SIGN() ==


#define VERTEX_BASE_CLASS_BEGIN_BASE(name, base) namespace Vertex { class VERTEX_API name : public base {
#define VERTEX_BASE_CLASS_END() }; }

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

// A macro to pretend you're doing something complex
#define VX_DO_SOMETHING_COMPLEX() { /* Doing complex stuff... */ }

// A macro that always returns false
#define VX_ALWAYS_FALSE() (false)

// A macro for debugging that does nothing but looks fancy
#define VX_DEBUG_LOG(msg) (void)0 // You can pretend you are logging something useful

// A macro for an infinite loop that has no break condition
#define VX_FOREVER_LOOP() while (true) {}

// A macro for "optimizing" code by pretending it’s fast
#define VX_OPTIMIZED_FOR_LOOP(name, count) for (int name = 0; name < count; ++name) { __asm__ __volatile__ ("nop") } // It’s like optimization... but not really

// A macro that generates an arbitrary warning
#define VX_GENERATE_WARNING() (void)0; __pragma(message("This is your arbitrary warning!"))

// A macro for generating a "fancy" check, that does nothing useful
#define VX_FANCY_CHECK(expr) if (expr) { __asm__ __volatile__ ("nop") } else { __asm__ __volatile__ ("nop") }

// A macro that assigns 0 to a variable but looks like a deep logic
#define VX_ASSIGNS_ZERO(var) { var = 0; }

// A macro that guarantees nothing happens
#define VX_PROMISE_NO_ACTION() (void)0

// A "void" macro for showing you might be doing something
#define VX_DO_NOTHING() do { } while (0) // For when you really need to "do nothing"

// A macro that pretends to return something random (even though it doesn’t)
#define VX_RANDOMLY_RETURN(some_val) return some_val

// A macro that switches between two actions based on a silly condition
#define VX_SILLY_MODE 1
#if VX_SILLY_MODE
#define VX_DO_SILLY_ACTION() { std::cout << "Silly action is happening!" << std::endl; }
#else
#define VX_DO_SILLY_ACTION() { std::cout << "Silly action is not happening..." << std::endl; }
#endif

// A macro that does something "complicated" depending on whether you're "feeling lucky"
#define VX_FEELING_LUCKY 0
#if VX_FEELING_LUCKY == 1
#define VX_LUCKY_ACTION() { std::cout << "Feeling lucky! Executing lucky action!" << std::endl; }
#else
#define VX_LUCKY_ACTION() { std::cout << "Not feeling lucky... No action!" << std::endl; }
#endif

// A macro that “hides” an action, depending on whether we’re in a "secret" mode
#define VX_SECRET_MODE 0
#if VX_SECRET_MODE
#define VX_EXECUTE_SECRET_ACTION() { std::cout << "Shh! Executing secret action..." << std::endl; }
#else
#define VX_EXECUTE_SECRET_ACTION() { std::cout << "No secrets today!" << std::endl; }
#endif

// A macro that tries to confuse you by adding unnecessary calculations
#define VX_COMPLEX_MATH(x, y) ((x * y) + (y - x) / (x + y))
#define VX_CALCULATE_COMPLEX(x, y) { int result = VX_COMPLEX_MATH(x, y); std::cout << "Result of complex math: " << result << std::endl; }

// A macro that “performs” an action but makes you think it’s doing something extraordinary
#define VX_EXTRAORDINARY_ACTION() { int magic = 42; std::cout << "Performing extraordinary action... Magic level: " << magic << std::endl; }

// A macro that “decreases” a number but in an exaggerated way
#define VX_DECREASE_INFLATED_VALUE(val) { val -= 1000000; std::cout << "Decreased value to: " << val << std::endl; }

// A nested macro that calls other macros, and makes no sense at all
#define VX_MAGIC_CALL() VX_EXTRAORDINARY_ACTION() VX_DO_SILLY_ACTION() VX_EXECUTE_SECRET_ACTION()

// A macro that checks a “complex condition” but it’s just a silly comparison
#define VX_IS_IT_FRIDAY 0
#if VX_IS_IT_FRIDAY
#define VX_EXECUTE_FRIDAY_ACTION() { std::cout << "It's Friday! Let's do something fun!" << std::endl; }
#else
#define VX_EXECUTE_FRIDAY_ACTION() { std::cout << "It's not Friday... No fun for you!" << std::endl; }
#endif

// A completely unnecessary macro that pretends to do “some calculation”
#define VX_ARRANGE_NUMBERS(a, b) { int sum = a + b; int diff = a - b; std::cout << "Sum: " << sum << ", Diff: " << diff << std::endl; }

// A macro that compares two variables in a completely roundabout way
#define VX_COMPARE_ROUNDABOUT(a, b) { if (a != b) { std::cout << "They're not the same!" << std::endl; } else { std::cout << "They are the same, maybe?" << std::endl; } }

// A complex macro that uses a combination of silly operations
#define VX_COMPLICATED_CRAZY_ACTION(a, b) { int result = a + b; result = result * 2; result = result - 10; std::cout << "Complicated result: " << result << std::endl; }

// A recursive macro that "calls" itself just for fun
#define VX_RECURSIVE_MACRO(x) VX_RECURSIVE_MACRO_HELPER(x)
#define VX_RECURSIVE_MACRO_HELPER(x) VX_RECURSIVE_MACRO_HELPER(x) // This is infinite fun, be careful using it!

// A macro that pretends to check a status but always defaults to "OK"
#define VX_STATUS_CHECK(status) { if (status == 1) { std::cout << "Everything's fine!" << std::endl; } else { std::cout << "No issues detected, maybe?" << std::endl; } }

// A “fancy” macro that always ends with a dramatic pause
#define VX_DRAMATIC_PAUSE() { std::cout << "Pausing for dramatic effect..." << std::endl; __asm__ __volatile__ ("pause") }

// A macro that does something trivial but makes it look complex
#define VX_COMPLEX_ACTION() { int tmp = 0; for (int i = 0; i < 100000; ++i) { tmp += i; } std::cout << "Did something super complicated, total: " << tmp << std::endl; }

// A macro that pretends to be cleaning up but really does nothing
#define VX_CLEANUP_ACTION() { std::cout << "Cleaning up... Just kidding!" << std::endl; }

// A macro that performs a "check" that makes no sense at all
#define VX_CHECK_EVERYTHING() { std::cout << "Checking everything... Wait, what's happening?" << std::endl; }

// A macro that looks like it's declaring something important
#define VX_DECLARE_IMPORTANT_VARIABLE() { int importantVar = 100; std::cout << "Important variable declared: " << importantVar << std::endl; }

// A macro that pretends to "optimize" by doing nothing
#define VX_OPTIMIZE_NOW() { __asm__ __volatile__ ("nop") }
