#include "vxpch.h"
#include "lua_script.h"
#include "Vertex/Lua/LuaScripting.h"
#include "Vertex/Core/FileSystem.h"
#include <imgui.h>
#include "Vertex/ImGui/ImGuiLink.h"
#include "Vertex/Utils/PlatformUtils.h"

namespace Vertex
{
	void BEHAVIOURLuaScript::OnCreate()
	{
		if(!m_Entity->GetScene()->IsEditor())
		{
			this->m_ScriptHandle = LuaScripting::NewScript(m_Entity->name());

			Buffer* buffer = FileSystem::ReadFileBinary(this->filepath);
			
			if (!buffer) { VX_CORE_ASSERT(0, "buffer error"); return; }

			VX_CORE_ASSERT(LuaScripting::LoadScript(this->m_ScriptHandle, buffer));

			LuaScripting::SetScriptBehaviour(this->m_ScriptHandle, this);
		}
	}
	void BEHAVIOURLuaScript::OnDestroy()
	{
	}
	void BEHAVIOURLuaScript::OnUpdate(Timestep& ts)
	{
		LuaScripting::UpdateScript(this->m_ScriptHandle, ts);
	}

	void BEHAVIOURLuaScript::ImGuiDrawProperties()
	{
		if (ImGui::TreeNodeEx((m_Entity->GetID() + "Lua Script").c_str(), ImGuiTreeNodeFlags_DefaultOpen, "Lua Script"))
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), filepath.string().c_str());
			if (ImGuiLink::InputText("File Path", buffer, sizeof(buffer)))
			{
				filepath = std::fs::path(buffer);
			}
			ImGui::SameLine();
			if (ImGui::Button("E"))
			{
				filepath = FileDialogs::OpenFile("Lua Script (*.lua)\0*.lua\0");
			}
			ImGuiLink::TreePop();
		}

		//char buffer[256];
		//memset(buffer, 0, sizeof(buffer));
		//strcpy_s(buffer, sizeof(buffer), tag.c_str());
		//if (ImGuiLink::InputText("Tag", buffer, sizeof(buffer)))
		//{
		//	tag = std::string(buffer);
		//}
	}

	VX_REGISTER_BEHAVIOUR(BEHAVIOURLuaScript, lua_script);
}