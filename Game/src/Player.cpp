#include "Player.h"
#include <Vertex/Renderer/Renderer2D.h>
#include <Vertex/Core/Logger.h>
#include "Enemy.h"
#include "EditorLayer.h"

namespace Vertex {

	float GetDist(const glm::vec2& current, const glm::vec2& target)
	{
		glm::vec2 toVector = target - current;
		float dist = glm::length(toVector);
		return dist;
	}

	Enemy* GetClosestEnemy(glm::vec2 pos)
	{
		Enemy* currentEnemy = NULL;
		float minDist = 5;
		glm::vec2 currentPos = pos;

		std::vector< Entity*> Entitys = EditorLayer::Get().GetEntitys();

		for (Entity* e : Entitys)
		{
			if (e->getEntityType() == ENTITY_TYPE_EMEMY)
			{
				float dist = GetDist(currentPos, e->getPos());
				if (dist < minDist)
				{
					currentEnemy = (Enemy*)e;
					minDist = dist;
				}
			}
		}
		return currentEnemy;
		
	}

	float fr = 0.1f;

	Player::Player() : Entity(glm::vec2(0, 0), Texture2D::Create("assets/textures/Player.png"))
	{
		id = get_uuid();
		m_scale = glm::vec2(1, 1);
	}

	Ref<Texture2D> Player::getTex()
	{
		return m_tex;
	}

	float timer = 0.0f;
	void Player::update(Timestep ts)
	{
		timer += ts;
		if (timer >= fr)
		{
			Enemy* e = GetClosestEnemy(m_pos);
			if (e != nullptr)
			{
				e->Damage(1);
			}

			
		}
		
	}

	void Player::draw(Timestep ts)
	{
		
		Renderer2D::DrawRotatedQuad(m_pos, m_scale, m_angle, m_tex);
	}

	uint16_t Player::getEntityType()
	{
		return ENTITY_TYPE_PLAYER;
	}

}