#include "Entity.h"

namespace Vertex {

	Entity::Entity(glm::vec2 pos, Ref<Texture2D> tex)
	{
		m_pos = pos;
		m_tex = tex;
	}

	Entity::Entity(glm::vec2 pos)
	{
		m_pos = pos;
	}

	Entity::Entity()
	{
		m_pos = glm::vec2(0,0);
	}

	void Entity::setPos(float x, float y)
	{
		m_pos.x = x;
		m_pos.y = y;
	}

	void Entity::setAngle(float angle)
	{
		m_angle = angle;
	}

	void Entity::setScale(float w, float h)
	{
		m_scale.x = w;
		m_scale.y = h;
	}

}