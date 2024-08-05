#pragma once

#include "Vertex/Core.h"

#include <string>
#include <functional>

namespace Vertex {

	// Events in Vertex are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { VX_PROFILE_FUNCTION(); return EventType::##type; }\
								virtual EventType GetEventType() const override { VX_PROFILE_FUNCTION(); return GetStaticType(); }\
								virtual const char* GetName() const override { VX_PROFILE_FUNCTION(); return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class VERTEX_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool handled = false;

		inline bool IsInCategory(EventCategory category)
		{
			VX_PROFILE_FUNCTION();
			return GetCategoryFlags() & category;
		}
	protected:
		
	};

	class VERTEX_API EventDispatcher
	{
		
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
			VX_PROFILE_FUNCTION();
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			VX_PROFILE_FUNCTION();
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		VX_PROFILE_FUNCTION();
		return os << e.ToString();
	}
}