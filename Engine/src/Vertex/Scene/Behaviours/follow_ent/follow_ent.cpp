#include <vxpch.h>

#include "follow_ent.h"
#include "Vertex/ImGui/ImGuiLink.h"

static glm::vec3 MoveTowards(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta) {
	glm::vec3 direction = target - current;
	float distance = glm::length(direction);

	// If the distance is smaller than the maxDelta, just return the target
	if (distance <= maxDistanceDelta || distance == 0.0f) {
		return target;
	}

	// Normalize the direction and move in that direction
	direction = glm::normalize(direction);
	return current + direction * maxDistanceDelta;
}

namespace Vertex
{
	void BEHAVIOURFollowENT::OnUpdate(Timestep& ts)
	{
		if (ent)
		{
			m_Entity->pos = MoveTowards(m_Entity->pos, ent->pos, speed * ts);
		}
	}

	void BEHAVIOURFollowENT::ImGuiDrawProperties()
	{
		if (ImGui::TreeNodeEx((m_Entity->GetID() + "follow_ent").c_str(), ImGuiTreeNodeFlags_DefaultOpen, "follow ent"))
		{
			//ENT_DRAG_DROP
			
			if (ent)
			{
				if (ImGui::Button(ent->name().c_str()))
				{
					ent = nullptr;
				}
			}
			else
			{
				ImGui::Button("NULL");
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENT_DRAG_DROP"))
				{
					UUID id = *(UUID*)payload->Data;
					for (Entity* ent : *m_Entity->GetScene())
					{
						if (ent->GetID() == id)
						{
							this->ent = ent;
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::TreePop();
		}
	}

	VX_REGISTER_BEHAVIOUR(BEHAVIOURFollowENT, follow_ent);
}


