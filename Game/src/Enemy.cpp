#include "Enemy.h"

namespace Vertex
{

	Enemy::Enemy()
	{
	}

	Ref<Texture2D> Enemy::getTex()
	{
		return m_tex;
	}

	void Enemy::update(Timestep ts)
	{
	}

	void Enemy::draw(Timestep ts)
	{

	}

	uint16_t Enemy::getEntityType()
	{
		return ENTITY_TYPE_EMEMY;
	}

}