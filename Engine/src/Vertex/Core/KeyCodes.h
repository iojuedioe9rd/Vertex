#pragma once

#include "Vertex/Core/Base.h"

namespace Vertex
{
	VERTEX_API typedef enum class  KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	
}

// From glfw3.h
#define VX_KEY_SPACE           ::Vertex::Key::Space
#define VX_KEY_APOSTROPHE      ::Vertex::Key::Apostrophe    /* ' */
#define VX_KEY_COMMA           ::Vertex::Key::Comma         /* , */
#define VX_KEY_MINUS           ::Vertex::Key::Minus         /* - */
#define VX_KEY_PERIOD          ::Vertex::Key::Period        /* . */
#define VX_KEY_SLASH           ::Vertex::Key::Slash         /* / */
#define VX_KEY_0               ::Vertex::Key::D0
#define VX_KEY_1               ::Vertex::Key::D1
#define VX_KEY_2               ::Vertex::Key::D2
#define VX_KEY_3               ::Vertex::Key::D3
#define VX_KEY_4               ::Vertex::Key::D4
#define VX_KEY_5               ::Vertex::Key::D5
#define VX_KEY_6               ::Vertex::Key::D6
#define VX_KEY_7               ::Vertex::Key::D7
#define VX_KEY_8               ::Vertex::Key::D8
#define VX_KEY_9               ::Vertex::Key::D9
#define VX_KEY_SEMICOLON       ::Vertex::Key::Semicolon     /* ; */
#define VX_KEY_EQUAL           ::Vertex::Key::Equal         /* = */
#define VX_KEY_A               ::Vertex::Key::A
#define VX_KEY_B               ::Vertex::Key::B
#define VX_KEY_C               ::Vertex::Key::C
#define VX_KEY_D               ::Vertex::Key::D
#define VX_KEY_E               ::Vertex::Key::E
#define VX_KEY_F               ::Vertex::Key::F
#define VX_KEY_G               ::Vertex::Key::G
#define VX_KEY_H               ::Vertex::Key::H
#define VX_KEY_I               ::Vertex::Key::I
#define VX_KEY_J               ::Vertex::Key::J
#define VX_KEY_K               ::Vertex::Key::K
#define VX_KEY_L               ::Vertex::Key::L
#define VX_KEY_M               ::Vertex::Key::M
#define VX_KEY_N               ::Vertex::Key::N
#define VX_KEY_O               ::Vertex::Key::O
#define VX_KEY_P               ::Vertex::Key::P
#define VX_KEY_Q               ::Vertex::Key::Q
#define VX_KEY_R               ::Vertex::Key::R
#define VX_KEY_S               ::Vertex::Key::S
#define VX_KEY_T               ::Vertex::Key::T
#define VX_KEY_U               ::Vertex::Key::U
#define VX_KEY_V               ::Vertex::Key::V
#define VX_KEY_W               ::Vertex::Key::W
#define VX_KEY_X               ::Vertex::Key::X
#define VX_KEY_Y               ::Vertex::Key::Y
#define VX_KEY_Z               ::Vertex::Key::Z
#define VX_KEY_LEFT_BRACKET    ::Vertex::Key::LeftBracket   /* [ */
#define VX_KEY_BACKSLASH       ::Vertex::Key::Backslash     /* \ */
#define VX_KEY_RIGHT_BRACKET   ::Vertex::Key::RightBracket  /* ] */
#define VX_KEY_GRAVE_ACCENT    ::Vertex::Key::GraveAccent   /* ` */
#define VX_KEY_WORLD_1         ::Vertex::Key::World1        /* non-US #1 */
#define VX_KEY_WORLD_2         ::Vertex::Key::World2        /* non-US #2 */

/* Function keys */
#define VX_KEY_ESCAPE          ::Vertex::Key::Escape
#define VX_KEY_ENTER           ::Vertex::Key::Enter
#define VX_KEY_TAB             ::Vertex::Key::Tab
#define VX_KEY_BACKSPACE       ::Vertex::Key::Backspace
#define VX_KEY_INSERT          ::Vertex::Key::Insert
#define VX_KEY_DELETE          ::Vertex::Key::Delete
#define VX_KEY_RIGHT           ::Vertex::Key::Right
#define VX_KEY_LEFT            ::Vertex::Key::Left
#define VX_KEY_DOWN            ::Vertex::Key::Down
#define VX_KEY_UP              ::Vertex::Key::Up
#define VX_KEY_PAGE_UP         ::Vertex::Key::PageUp
#define VX_KEY_PAGE_DOWN       ::Vertex::Key::PageDown
#define VX_KEY_HOME            ::Vertex::Key::Home
#define VX_KEY_END             ::Vertex::Key::End
#define VX_KEY_CAPS_LOCK       ::Vertex::Key::CapsLock
#define VX_KEY_SCROLL_LOCK     ::Vertex::Key::ScrollLock
#define VX_KEY_NUM_LOCK        ::Vertex::Key::NumLock
#define VX_KEY_PRINT_SCREEN    ::Vertex::Key::PrintScreen
#define VX_KEY_PAUSE           ::Vertex::Key::Pause
#define VX_KEY_F1              ::Vertex::Key::F1
#define VX_KEY_F2              ::Vertex::Key::F2
#define VX_KEY_F3              ::Vertex::Key::F3
#define VX_KEY_F4              ::Vertex::Key::F4
#define VX_KEY_F5              ::Vertex::Key::F5
#define VX_KEY_F6              ::Vertex::Key::F6
#define VX_KEY_F7              ::Vertex::Key::F7
#define VX_KEY_F8              ::Vertex::Key::F8
#define VX_KEY_F9              ::Vertex::Key::F9
#define VX_KEY_F10             ::Vertex::Key::F10
#define VX_KEY_F11             ::Vertex::Key::F11
#define VX_KEY_F12             ::Vertex::Key::F12
#define VX_KEY_F13             ::Vertex::Key::F13
#define VX_KEY_F14             ::Vertex::Key::F14
#define VX_KEY_F15             ::Vertex::Key::F15
#define VX_KEY_F16             ::Vertex::Key::F16
#define VX_KEY_F17             ::Vertex::Key::F17
#define VX_KEY_F18             ::Vertex::Key::F18
#define VX_KEY_F19             ::Vertex::Key::F19
#define VX_KEY_F20             ::Vertex::Key::F20
#define VX_KEY_F21             ::Vertex::Key::F21
#define VX_KEY_F22             ::Vertex::Key::F22
#define VX_KEY_F23             ::Vertex::Key::F23
#define VX_KEY_F24             ::Vertex::Key::F24
#define VX_KEY_F25             ::Vertex::Key::F25

/* Keypad */
#define VX_KEY_KP_0            ::Vertex::Key::KP0
#define VX_KEY_KP_1            ::Vertex::Key::KP1
#define VX_KEY_KP_2            ::Vertex::Key::KP2
#define VX_KEY_KP_3            ::Vertex::Key::KP3
#define VX_KEY_KP_4            ::Vertex::Key::KP4
#define VX_KEY_KP_5            ::Vertex::Key::KP5
#define VX_KEY_KP_6            ::Vertex::Key::KP6
#define VX_KEY_KP_7            ::Vertex::Key::KP7
#define VX_KEY_KP_8            ::Vertex::Key::KP8
#define VX_KEY_KP_9            ::Vertex::Key::KP9
#define VX_KEY_KP_DECIMAL      ::Vertex::Key::KPDecimal
#define VX_KEY_KP_DIVIDE       ::Vertex::Key::KPDivide
#define VX_KEY_KP_MULTIPLY     ::Vertex::Key::KPMultiply
#define VX_KEY_KP_SUBTRACT     ::Vertex::Key::KPSubtract
#define VX_KEY_KP_ADD          ::Vertex::Key::KPAdd
#define VX_KEY_KP_ENTER        ::Vertex::Key::KPEnter
#define VX_KEY_KP_EQUAL        ::Vertex::Key::KPEqual

#define VX_KEY_LEFT_SHIFT      ::Vertex::Key::LeftShift
#define VX_KEY_LEFT_CONTROL    ::Vertex::Key::LeftControl
#define VX_KEY_LEFT_ALT        ::Vertex::Key::LeftAlt
#define VX_KEY_LEFT_SUPER      ::Vertex::Key::LeftSuper
#define VX_KEY_RIGHT_SHIFT     ::Vertex::Key::RightShift
#define VX_KEY_RIGHT_CONTROL   ::Vertex::Key::RightControl
#define VX_KEY_RIGHT_ALT       ::Vertex::Key::RightAlt
#define VX_KEY_RIGHT_SUPER     ::Vertex::Key::RightSuper
#define VX_KEY_MENU            ::Vertex::Key::Menu