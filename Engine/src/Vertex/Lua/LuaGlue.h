#pragma once
#include <Vertex/Core/Base.h>

namespace sol
{
	class state;
}

namespace Vertex
{
	class LuaScript;
	class VERTEX_API LuaGlue
	{
	public:
		

	private:
		static void PreSetupLuaState(LuaScript* script);
		static void PostSetupLuaState(LuaScript* script);

		friend class LuaScripting;
	};

	
}