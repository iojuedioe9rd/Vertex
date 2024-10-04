#include "base_npc.h"
#include <glm/gtx/norm.hpp> // For glm::length2
#include <Vertex/Core/Logger.h>

namespace Vertex {



	glm::vec3 moveTowards(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta) {
		glm::vec3 direction = target - current;

		// Calculate the distance between current and target
		float distance = glm::length(direction);

		// If we are already at the target, return it
		if (distance == 0 || maxDistanceDelta >= distance) {
			return target;
		}

		// Normalize the direction vector and scale it by maxDistanceDelta
		glm::vec3 normalizedDirection = direction / distance;
		return current + normalizedDirection * maxDistanceDelta;
	}

	ENTBaseNPC::~ENTBaseNPC()
	{
	}

	void ENTBaseNPC::InIt(const std::vector<std::vector<uint8_t>>& grid)
	{
		this->base_npc_init = true;
		this->base_npc_pathfinding = new Pathfinding(grid);
	}

	void ENTBaseNPC::UpdateNPC(Timestep& ts)
	{
		if (!base_npc_init || current == nullptr) return;
		Node start = { pos, 0, 0 };

		std::vector<Node> path = base_npc_pathfinding->aStar(start, *current);

		if (!path.empty()) {
			glm::vec3 p;
			float m = 9999;
			for (const Node& node : path) {
				float dis = (glm::vec2(pos) - glm::vec2(node.position.x, node.position.y)).length();

				if (dis <= m)
				{
					m = dis;
					p = glm::vec3(node.position.x, node.position.y, 0);
				}
			}

			VX_INFO("Going To {x: {0}, y: {1}}", p.x, p.y);
			pos = moveTowards(pos, p, speed * ts);
		}

	}

}