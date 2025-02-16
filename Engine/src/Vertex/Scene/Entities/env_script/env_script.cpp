#include <vxpch.h>

#include "env_script.h"
//#include "../../../MainLib.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "Vertex/Scripting/ScriptEngine.h"

namespace Vertex
{
	void ENTEnvScript::Update(Timestep& ts)
	{
		

		ScriptEngine::OnUpdateEntity(this, ts);
	}

	void ENTEnvScript::Draw(Timestep& ts)
	{
		

		if(!m_Scene->IsEditor())
			ScriptEngine::OnDrawEntity(this);
	}

	void ENTEnvScript::OnImGuiDraw()
	{
		if (!m_Scene->IsEditor())
		{
			ScriptEngine::OnImGuiDrawEntity(this);
		}
			
	}

	void ENTEnvScript::PhysUpdate(Timestep& ts)
	{
		ScriptEngine::OnPhysUpdateEntity(this, ts);
	}

	ENT_REGISTER_ENTITY(ENTEnvScript, env_script);
}