#include "env_script.h"
#include "../../../MainLib.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "../../../Scripting/ScriptEngine.h"

namespace Vertex
{
	void ENTEnvScript::Update(Timestep& ts)
	{
		if (!(VXEntities_GET_FLAGS() & VXEntities_INIT_USE_MONO))
		{
			VX_CORE_CRITICAL("Mono Not Init");
			return;
		}

		ScriptEngine::OnUpdateEntity(this, ts);
	}

	void ENTEnvScript::Draw(Timestep& ts)
	{
		if (!(VXEntities_GET_FLAGS() & VXEntities_INIT_USE_MONO))
		{
			VX_CORE_CRITICAL("Mono Not Init");
			return;
		}

		if(!m_Scene->IsEditor())
			ScriptEngine::OnDrawEntity(this);
	}

	void ENTEnvScript::PhysUpdate(Timestep& ts)
	{
		ScriptEngine::OnPhysUpdateEntity(this, ts);
	}
}