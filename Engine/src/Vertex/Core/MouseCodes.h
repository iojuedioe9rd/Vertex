#pragma once
#include <Vertex/Core/Base.h>

namespace Vertex {

	VERTEX_API typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define VX_MOUSE_BUTTON_0      ::Vertex::Mouse::Button0
#define VX_MOUSE_BUTTON_1      ::Vertex::Mouse::Button1
#define VX_MOUSE_BUTTON_2      ::Vertex::Mouse::Button2
#define VX_MOUSE_BUTTON_3      ::Vertex::Mouse::Button3
#define VX_MOUSE_BUTTON_4      ::Vertex::Mouse::Button4
#define VX_MOUSE_BUTTON_5      ::Vertex::Mouse::Button5
#define VX_MOUSE_BUTTON_6      ::Vertex::Mouse::Button6
#define VX_MOUSE_BUTTON_7      ::Vertex::Mouse::Button7
#define VX_MOUSE_BUTTON_LAST   ::Vertex::Mouse::ButtonLast
#define VX_MOUSE_BUTTON_LEFT   ::Vertex::Mouse::ButtonLeft
#define VX_MOUSE_BUTTON_RIGHT  ::Vertex::Mouse::ButtonRight
#define VX_MOUSE_BUTTON_MIDDLE ::Vertex::Mouse::ButtonMiddle