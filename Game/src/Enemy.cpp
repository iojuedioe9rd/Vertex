#include "Enemy.h"
#include <Vertex/Core/Logger.h>
#include "EditorLayer.h"
#include <Vertex/Renderer/Renderer2D.h>
#include <glm/gtc/constants.hpp> // For glm::length()

#include "rand.h"

glm::vec2 moveTowards(const glm::vec2& current, const glm::vec2& target, float maxDistanceDelta) {
	glm::vec2 toVector = target - current;
	float dist = glm::length(toVector);

	if (dist <= maxDistanceDelta || dist < glm::epsilon<float>()) {
		return target;
	}

	return current + toVector * (maxDistanceDelta / dist);
}

float GetDist(const glm::vec2& current, const glm::vec2& target)
{
	glm::vec2 toVector = target - current;
	float dist = glm::length(toVector);
	return dist;
}

namespace Vertex
{
	float rngOffset;
	int nextPathCellIndex = 0;

	float lerp(float a, float b, float t)
	{
		return a + (b - a) * t; //This returns a + t percent (t = 0.f is 0% and t = 1.f is 100%) of b
	}
	float GetSize(int health, int healthMax, float speed, float speedMax, float sizeMod)
	{
		float size = 0;

		size += lerp(0, 1, (float)health / (float)healthMax);
		size += lerp(0, 1, (float)(speed - .1) / (float)(speedMax - .1));

		size = lerp(0.1, 1, size / 2) + (sizeMod);

		return size;
	}

	Enemy::Enemy(int health, float speed, float sizeMod)
	{
		rngOffset = static_cast<float>(rand(0, 100)) / static_cast<float>(100);
		id = get_uuid();
		m_tex = Texture2D::Create("assets/textures/Enemy.png");

		m_pos = glm::vec2(-100, 0);
		meeeeeeeee = new EnemyS();
		meeeeeeeee->health = health;
		meeeeeeeee->healthMax = health;

		meeeeeeeee->speed = speed;
		meeeeeeeee->speedMax = speed;
	}

	Enemy::~Enemy()
	{
		delete meeeeeeeee;
	}

	Ref<Texture2D> Enemy::getTex()
	{
		return m_tex;
	}

	float t2 = 5;

	void Enemy::update(Timestep ts)
	{
		t2--;
		if (true)
		{

			std::vector<glm::i32vec2> pathCells = EditorLayer::Get().GetGridManager()->GetPathCells();

			
			m_pos = moveTowards(m_pos, pathCells[nextPathCellIndex], meeeeeeeee->speed * ts);

			VX_INFO("{0}", pathCells.size());


			if (GetDist(m_pos, pathCells[nextPathCellIndex]) <= 0.1)
			{

				nextPathCellIndex++;
				if (nextPathCellIndex >= pathCells.size())
				{
					Damage(1000 * meeeeeeeee->health);
				}
			}
			
		}
	}

	void Enemy::draw(Timestep ts)
	{
		glm::vec2 size = glm::vec2(1, 1) * GetSize(meeeeeeeee->health, meeeeeeeee->healthMax, meeeeeeeee->speed, meeeeeeeee->speedMax, meeeeeeeee->sizeMod);
		VX_INFO("{0}, {1}", size.x, size.y);
		Renderer2D::DrawRotatedQuad(m_pos, size, 0, m_tex);
	}

	uint16_t Enemy::getEntityType()
	{
		return ENTITY_TYPE_EMEMY;
	}

	void Enemy::Damage(int amount)
	{
		if (this == nullptr || (int)this == 0x1) { return; }
		float ogSpeed = meeeeeeeee->speed;
		if (meeeeeeeee->speed > .1f)
		{
			
			meeeeeeeee->speed -= amount;
			meeeeeeeee->speed = (meeeeeeeee->speed > .1f) ? meeeeeeeee->speed : .1f;

		}

		

		if (meeeeeeeee->speed <= .1f)
		{
			meeeeeeeee->health -= ogSpeed - amount * -1;
			if (meeeeeeeee->health <= 0)
			{
				EditorLayer::Get().RemoveE(this);
			}
		}

		meeeeeeeee->sizeMod -= amount / meeeeeeeee->sizeModMax;

		
	}

	int Enemy::GetHealth()
	{
		return meeeeeeeee->health;
	}

}