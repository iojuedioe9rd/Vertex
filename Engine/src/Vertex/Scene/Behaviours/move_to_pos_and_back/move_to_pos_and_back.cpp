#include "vxpch.h"
#include "move_to_pos_and_back.h"
#include <imgui.h>
#include <Vertex/ImGui/ImGuiLink.h>


static glm::vec3 MoveTowards(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta) {
	glm::vec3 direction = target - current;
	float distance = glm::length(direction);

	if (distance <= maxDistanceDelta || glm::abs(distance) < glm::epsilon<float>()) {
		return target;
	}

	return current + (direction / distance) * maxDistanceDelta;  // Normalize safely
}

namespace Vertex
{
	VX_REGISTER_BEHAVIOUR(BEHAVIOURMoveToPosAndBack, move_to_pos_and_back);

	void BEHAVIOURMoveToPosAndBack::OnUpdate(Timestep& ts)
	{
		

		if (glm::distance(m_Entity->pos, (m_Dir ? gotoPos : m_InitialPosition)) < 0.3f)
		{
			m_Entity->pos = (m_Dir ? gotoPos : m_InitialPosition);
			m_Dir = !m_Dir;
		}
		if (m_Dir)
		{
			m_Entity->pos = MoveTowards(m_Entity->pos, gotoPos, speed * ts);
		}
		else
		{
			m_Entity->pos = MoveTowards(m_Entity->pos, m_InitialPosition, speed * ts);
		}
	}
	void BEHAVIOURMoveToPosAndBack::ImGuiDrawProperties()
	{
		if (ImGui::TreeNodeEx((m_Entity->GetID() + "Moving Platform").c_str(), ImGuiTreeNodeFlags_DefaultOpen, "Moving Platform"))
		{
			ImGuiLink::DrawVec3Control("Goto Pos", gotoPos);
			ImGuiLink::DragFloat("Speed", &speed);
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
}

