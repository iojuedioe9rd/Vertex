#pragma once

#include "Event.h"
#include "Vertex/Debug/Instrumentor.h"
#include <sstream>

namespace Vertex {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { VX_PROFILE_FUNCTION(); return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) { VX_PROFILE_FUNCTION(); }

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) { VX_PROFILE_FUNCTION(); }

		inline int GetRepeatCount() const { VX_PROFILE_FUNCTION(); return m_RepeatCount; }

		std::string ToString() const override
		{
			VX_PROFILE_FUNCTION();
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) { VX_PROFILE_FUNCTION(); }

		std::string ToString() const override
		{
			VX_PROFILE_FUNCTION();
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	typedef unsigned char uchar;

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const uchar keycode)
			: KeyEvent(keycode) { VX_PROFILE_FUNCTION(); }

		std::string ToString() const override
		{
			VX_PROFILE_FUNCTION();
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}