#include <vxpch.h>
#include "trigger_lava.h"
#include "Vertex/Scene/Entities/env_script/env_script.h"
#include "Vertex/Renderer/Renderer2D.h"

namespace Vertex
{
	ENT_REGISTER_ENTITY(ENTTriggerLava, trigger_lava);

	void ENTTriggerLava::Update(Timestep& ts)
	{
		if (Type != ENTBaseRigidbody2D::BodyType::Static)
		{
			Type = ENTBaseRigidbody2D::BodyType::Static;
		}

		ENTBaseRigidbody2D::Update(ts);
	}

	void ENTTriggerLava::Draw(Timestep& ts)
	{
		Renderer2D::DrawQuad(pos, size, glm::vec4(1, 0, 0, 1));
	}

	void ENTTriggerLava::BeginContact(Entity* entity)
	{
		ENTEnvScript* script = dynamic_cast<ENTEnvScript*>(entity);

		if (script)
		{
			script->OnLava();
		}
	}

	void ENTTriggerLava::PhysUpdate(Timestep& ts)
	{
		ENTBaseRigidbody2D::PhysUpdate(ts);
	}

	void ENTTriggerLava::EndContact(Entity* entity)
	{

	}
}