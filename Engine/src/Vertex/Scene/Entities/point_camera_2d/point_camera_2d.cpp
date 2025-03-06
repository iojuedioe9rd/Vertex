#include <vxpch.h>
#include "point_camera_2d.h"


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

static glm::vec2 MoveTowards(const glm::vec2& current, const glm::vec2& target, float maxDistanceDelta) {
	glm::vec2 direction = target - current;
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
	void ENTPointCamera2D::Update(Timestep& ts)
	{
		Entity* player = m_Scene->FindEntityByName("Player");

		if (player)
		{
			glm::vec2 newPos = pos;
			newPos = MoveTowards(newPos, player->pos, 5.0f);
			pos.x = newPos.x;
			pos.y = newPos.y;
		}
	}

	ENT_REGISTER_ENTITY(ENTPointCamera2D, point_camera_2d);
	ENT_REGISTER_ENTITY(ENTPointCamera2D, point_camera_3d);
	ENT_REGISTER_ENTITY(ENTPointCamera2D, point_camera);
}