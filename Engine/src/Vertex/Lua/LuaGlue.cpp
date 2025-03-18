#include <vxpch.h>
#include "LuaScripting.h"
#include "LuaGlue.h"
#include <Vertex/Core/Input.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "ArenaAllocator.h"
#include "sol/config.hpp"
#include "sol/forward.hpp"
#include "sol/sol.hpp"



static struct LuaVec3
{
	float x, y, z = 0;

	static LuaVec3 FromGLM(glm::vec3 pos)
	{
		LuaVec3 vec{};
		vec.x = pos.x;
		vec.y = pos.y;
		vec.z = pos.z;

		return vec;
	}

	static glm::vec3 ToGLM(LuaVec3 vec)
	{
		glm::vec3 glmVEC3{};
		glmVEC3.x = vec.x;
		glmVEC3.y = vec.y;
		glmVEC3.z = vec.z;

		return glmVEC3;
	}

	LuaVec3() {};
	LuaVec3(float x) : x(x) {}
	LuaVec3(float x, float y) : x(x), y(y) {}
	LuaVec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

static struct LuaVec2
{
    float x, y = 0;

    static LuaVec2 FromGLM(glm::vec2 pos)
    {
        LuaVec2 vec{};
        vec.x = pos.x;
        vec.y = pos.y;

        return vec;
    }

    static glm::vec2 ToGLM(LuaVec2 vec)
    {
        glm::vec2 glmVEC3{};
        glmVEC3.x = vec.x;
        glmVEC3.y = vec.y;

        return glmVEC3;
    }

    LuaVec2() {};
    LuaVec2(float x) : x(x) {}
    LuaVec2(float x, float y) : x(x), y(y) {}
};

namespace Vertex
{
    static void register_keycode_enum(sol::state& lua) {
        lua.new_enum<KeyCode>("KeyCode",
            { {"Space", KeyCode::Space},
             {"Apostrophe", KeyCode::Apostrophe},
             {"Comma", KeyCode::Comma},
             {"Minus", KeyCode::Minus},
             {"Period", KeyCode::Period},
             {"Slash", KeyCode::Slash},
             {"D0", KeyCode::D0},
             {"D1", KeyCode::D1},
             {"D2", KeyCode::D2},
             {"D3", KeyCode::D3},
             {"D4", KeyCode::D4},
             {"D5", KeyCode::D5},
             {"D6", KeyCode::D6},
             {"D7", KeyCode::D7},
             {"D8", KeyCode::D8},
             {"D9", KeyCode::D9},
             {"Semicolon", KeyCode::Semicolon},
             {"Equal", KeyCode::Equal},
             {"A", KeyCode::A},
             {"B", KeyCode::B},
             {"C", KeyCode::C},
             {"D", KeyCode::D},
             {"E", KeyCode::E},
             {"F", KeyCode::F},
             {"G", KeyCode::G},
             {"H", KeyCode::H},
             {"I", KeyCode::I},
             {"J", KeyCode::J},
             {"K", KeyCode::K},
             {"L", KeyCode::L},
             {"M", KeyCode::M},
             {"N", KeyCode::N},
             {"O", KeyCode::O},
             {"P", KeyCode::P},
             {"Q", KeyCode::Q},
             {"R", KeyCode::R},
             {"S", KeyCode::S},
             {"T", KeyCode::T},
             {"U", KeyCode::U},
             {"V", KeyCode::V},
             {"W", KeyCode::W},
             {"X", KeyCode::X},
             {"Y", KeyCode::Y},
             {"Z", KeyCode::Z},
             {"LeftBracket", KeyCode::LeftBracket},
             {"Backslash", KeyCode::Backslash},
             {"RightBracket", KeyCode::RightBracket},
             {"GraveAccent", KeyCode::GraveAccent},
             {"World1", KeyCode::World1},
             {"World2", KeyCode::World2},
             {"Escape", KeyCode::Escape},
             {"Enter", KeyCode::Enter},
             {"Tab", KeyCode::Tab},
             {"Backspace", KeyCode::Backspace},
             {"Insert", KeyCode::Insert},
             {"Delete", KeyCode::Delete},
             {"Right", KeyCode::Right},
             {"Left", KeyCode::Left},
             {"Down", KeyCode::Down},
             {"Up", KeyCode::Up},
             {"PageUp", KeyCode::PageUp},
             {"PageDown", KeyCode::PageDown},
             {"Home", KeyCode::Home},
             {"End", KeyCode::End},
             {"CapsLock", KeyCode::CapsLock},
             {"ScrollLock", KeyCode::ScrollLock},
             {"NumLock", KeyCode::NumLock},
             {"PrintScreen", KeyCode::PrintScreen},
             {"Pause", KeyCode::Pause},
             {"F1", KeyCode::F1},
             {"F2", KeyCode::F2},
             {"F3", KeyCode::F3},
             {"F4", KeyCode::F4},
             {"F5", KeyCode::F5},
             {"F6", KeyCode::F6},
             {"F7", KeyCode::F7},
             {"F8", KeyCode::F8},
             {"F9", KeyCode::F9},
             {"F10", KeyCode::F10},
             {"F11", KeyCode::F11},
             {"F12", KeyCode::F12},
             {"F13", KeyCode::F13},
             {"F14", KeyCode::F14},
             {"F15", KeyCode::F15},
             {"F16", KeyCode::F16},
             {"F17", KeyCode::F17},
             {"F18", KeyCode::F18},
             {"F19", KeyCode::F19},
             {"F20", KeyCode::F20},
             {"F21", KeyCode::F21},
             {"F22", KeyCode::F22},
             {"F23", KeyCode::F23},
             {"F24", KeyCode::F24},
             {"F25", KeyCode::F25},
             {"KP0", KeyCode::KP0},
             {"KP1", KeyCode::KP1},
             {"KP2", KeyCode::KP2},
             {"KP3", KeyCode::KP3},
             {"KP4", KeyCode::KP4},
             {"KP5", KeyCode::KP5},
             {"KP6", KeyCode::KP6},
             {"KP7", KeyCode::KP7},
             {"KP8", KeyCode::KP8},
             {"KP9", KeyCode::KP9},
             {"KPDecimal", KeyCode::KPDecimal},
             {"KPDivide", KeyCode::KPDivide},
             {"KPMultiply", KeyCode::KPMultiply},
             {"KPSubtract", KeyCode::KPSubtract},
             {"KPAdd", KeyCode::KPAdd},
             {"KPEnter", KeyCode::KPEnter},
             {"KPEqual", KeyCode::KPEqual},
             {"LeftShift", KeyCode::LeftShift},
             {"LeftControl", KeyCode::LeftControl},
             {"LeftAlt", KeyCode::LeftAlt},
             {"LeftSuper", KeyCode::LeftSuper},
             {"RightShift", KeyCode::RightShift},
             {"RightControl", KeyCode::RightControl},
             {"RightAlt", KeyCode::RightAlt},
             {"RightSuper", KeyCode::RightSuper},
             {"Menu", KeyCode::Menu}
            });
    }

	static void log_info(const std::string& log)
	{
		VX_CORE_INFO("{}", log.c_str());
	}

	static void set_pos(LuaScript* script, float x, float y, float z)
	{
		if (script && script->GetBehaviour() && script->GetBehaviour()->GetEntity())
		{
			script->GetBehaviour()->GetEntity()->pos = glm::vec3(x, y, z);
		}
		else
		{
			// Handle error (log, throw exception, etc.)
		}
	}
	static bool is_key_pressed(KeyCode c)
	{
		// Check if the corresponding key is pressed (you can map key codes to characters here)
		// Assuming key code mapping for character input:
		
		return Input::IsKeyPressed(c); // Reuse your existing is_key_pressed function
        
	}

	void LuaGlue::PreSetupLuaState(LuaScript* script)
	{
		// Create the input table in Lua and add functions to it
		
        register_keycode_enum(*script->m_State);

		// Bind the is_key_pressed function to the input table
        script->m_State->set_function("is_key_pressed", &is_key_pressed);

		

		script->m_State->set_function("log_info", &log_info);
		script->m_State->new_usertype<LuaVec3>("vec3",
			sol::constructors<
			LuaVec3(),
			LuaVec3(float),
			LuaVec3(float, float),
			LuaVec3(float, float, float)
			>(),
			"x", &LuaVec3::x,
			"y", &LuaVec3::y,
			"z", &LuaVec3::z
		);

        script->m_State->new_usertype<LuaVec2>("vec2",
            sol::constructors<
            LuaVec2(),
            LuaVec2(float),
            LuaVec2(float, float)
            >(),
            "x", &LuaVec2::x,
            "y", &LuaVec2::y
        );

		script->m_State->set_function("set_pos", [script](LuaVec3 vec3) {
			set_pos(script, vec3.x, vec3.y, vec3.z);
			});

		script->m_State->set_function("get_pos", [script]() {
			Entity* ent = script->GetBehaviour()->GetEntity();
			LuaVec3 vec3 = LuaVec3::FromGLM(ent->pos);
			return vec3;
		});

		
	}
	void LuaGlue::PostSetupLuaState(LuaScript* script)
	{
	}
}