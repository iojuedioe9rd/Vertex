#include "vxpch.h"
#include "LuaScripting.h"

#include "luacpp.hpp"



namespace Vertex
{
	struct LuaScriptingData
	{
		lua_State* testState = nullptr;
	};

	static LuaScriptingData* s_Data;

	void LuaScripting::Init()
	{
		s_Data = new LuaScriptingData();

		std::string cmd = "x = 7.53456 + 11.67558 + math.sin(23.7)";

		s_Data->testState = luaL_newstate();
		luaL_openlibs(s_Data->testState);

		int r = luaL_dostring(s_Data->testState, cmd.c_str());

		if (r == LUA_OK)
		{
			lua_getglobal(s_Data->testState, "x");
			if (lua_isnumber(s_Data->testState, -1))
			{
				float x = (float)lua_tonumber(s_Data->testState, -1);

				VX_INFO("X from LUA is {0}", x);
			}
		}
		else
		{
			std::string errormsg = lua_tostring(s_Data->testState, -1);
			VX_CORE_INFO(errormsg);
		}
	}

	void LuaScripting::Shutdown()
	{
		lua_close(s_Data->testState);
		s_Data->testState = nullptr;
		delete s_Data;
	}
}