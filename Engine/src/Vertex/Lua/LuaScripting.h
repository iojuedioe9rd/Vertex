#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/AssetManager/Asset.h"
#include <Vertex/Core/Buffer.h>
#include "Vertex/Lua/ArenaAllocator.h"
#include "Vertex/Scene/Entity.h"
#include "Vertex/Scene/Behaviour.h"
#include "Vertex/Scene/Behaviours/lua_script/lua_script.h"


class LuaContext;
namespace sol
{
	class state;
}


namespace Vertex
{


	using LuaScriptHandle = IntUUID;

	class VERTEX_API LuaScript : public Asset
	{
	public:
		virtual AssetType GetType() const override { return AssetType::LuaScript; }
		LuaScriptHandle GetScriptHandle() const { return m_ScriptHandle; }
		BEHAVIOURLuaScript* GetBehaviour() { return m_Behaviour; }
	private:

		LuaScript(LuaScriptHandle& Handle) : m_ScriptHandle(Handle) {}
		sol::state* m_State = nullptr;
		LuaScriptHandle m_ScriptHandle;
		std::string m_Name;
		ArenaAllocator* m_ArenaAllocator = nullptr;
		BEHAVIOURLuaScript* m_Behaviour = nullptr;

		friend class LuaScripting;
		friend class LuaGlue;
	};

	class VERTEX_API LuaScripting
	{
	public:

		enum class VERTEX_API LuaBufferType : uint8_t
		{
			Text,
			RawData,

		};

		static void Init();
		static void Shutdown();


		static LuaScriptHandle NewScript(std::string name);
		static bool LoadScript(LuaScriptHandle& handle, Buffer* buffer, LuaBufferType bufferType = LuaBufferType::Text);
		static bool UpdateScript(LuaScriptHandle& handle, Timestep& ts);

		static void RunTests();

		static void SetScriptBehaviour(LuaScriptHandle& handle, BEHAVIOURLuaScript* behaviour);
		 

	private:

		static bool LoadScript_Text(LuaScript* script, Buffer* buffer);

		static void PreSetupLuaState(LuaScript* script);
		static void PostSetupLuaState(LuaScript* script);

		friend class LuaScript;
	};

	
}