#include "npc_enemy.h"
#include "Vertex/Renderer/Renderer2D.h"

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

bool BoxCollision(glm::vec3 rect1Pos, glm::vec2 rect1Size, glm::vec3 rect2Pos, glm::vec2 rect2Size)
{

	
	glm::vec4 rect1 = glm::vec4(rect1Pos.x, rect1Pos.y, rect1Pos.z, rect1Size.x);
	glm::vec4 rect2 = glm::vec4(rect2Pos.x, rect2Pos.y, rect2Size.x, rect2Size.y);


	if (
		rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect1Size.y &&
		rect1.y + rect1Size.y > rect2.y
		) {
		return true;

	}



	return 0;
}

float distance(const glm::vec3& pointA, const glm::vec3& pointB) {
	return glm::length(pointA - pointB);
}

namespace Vertex
{


	ENTNpcEnemy::~ENTNpcEnemy()
	{
		delete m_Audio;

	}
	// Update the NPC's state
	void ENTNpcEnemy::Update(Timestep& ts) {
		if (m_player == nullptr || !base_npc_init) return;


		if (t2 >= 5)
		{
			for (ENTGuner* g : *m_player->GetGuner().get())
			{
				if (g == nullptr) continue;
				bool lol = 0;
				if (BoxCollision(pos, this->size, g->pos, g->size))
				{
					m_Audio->Play(); // Play audio

					float multiplier = m_player->GetHealthPercent() * 2.0f;
					m_player->Heal(g->size.x * 5 / (multiplier / 1.6f));
					Damage(1);
				}
				if (lol)
				{
					t2 = 0.0f;
				}
			}
		}
		else
		{
			t2++;
		}


		// Calculate the distance between the NPC and the player
		float dis = distance(pos, m_player->pos);


		// Exit if the distance is greater than 5
		if (dis > 12.0f) {
			return;
		}

		// Time-based logic for audio
		t += ts.GetSeconds();
		if (t >= maxTime) {
			maxTime *= 2; // Increase time between audio plays
			maxTime /= 1.4f;
			t = 0; // Reset time
			m_Audio->Play(); // Play audio
		}

		// Move towards the player
		glm::vec3 newPos = moveTowards(pos, m_player->pos, speed * ts.GetSeconds());
		if (!BoxCollision(newPos, this->size, m_player->pos, m_player->size)) {
			pos = newPos; // Only update if no collision occurs
		}


		if (BoxCollision(newPos, this->size, m_player->pos, m_player->size))
		{
			m_player->Die(newPos); // Handle player death if a collision occurs
			Damage(50);
			m_Audio->Play(); // Play audio
		}


	}


	void ENTNpcEnemy::Draw(Timestep& ts)
	{
		if (base_npc_init)
		{
			Renderer2D::DrawQuad(glm::vec2(pos.x, pos.y), glm::vec2(size.x, size.y), m_tex, 1, glm::vec4(1), true);
		}

	}
	void ENTNpcEnemy::OnDeath()
	{
		m_tilemap->RemoveEnemy(this);
		m_Audio2->Play();
		m_player->KilledEnemy();
		m_Scene->RemoveEntity(*this);

	}
}
