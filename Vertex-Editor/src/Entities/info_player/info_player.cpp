#include "info_player.h"
#include <Vertex/Renderer/Renderer2D.h>
#include <Vertex/Core/Input.h>

Ref<std::vector<ENTGuner*>> lol = Ref<std::vector<ENTGuner*>>();

glm::vec3 moveTowardss(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta) {
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

namespace Vertex {

	

	glm::vec3 NextPos(float speed, glm::vec3 pos, Timestep& ts)
	{
		
		if (Input::IsKeyPressed(KeyCode::W))
		{
			pos.y += speed * ts + sin(ts);

		}
		if (Input::IsKeyPressed(KeyCode::S))
		{
			pos.y -= speed * ts + sin(ts);

		}
		if (Input::IsKeyPressed(KeyCode::A))
		{
			pos.x -= speed * ts + sin(ts);

		}
		if (Input::IsKeyPressed(KeyCode::D))
		{
			pos.x += speed * ts + sin(ts);

		}
		return pos;
	}

	Ref<std::vector<ENTGuner*>> ENTInfoPlayer::GetGuner()
	{
		return lol;
	}

	void ENTInfoPlayer::pass()
	{
		
		lol.reset(new std::vector<ENTGuner*>());
	}

	void ENTInfoPlayer::Update(Timestep& ts)
	{

		if (hasThePlayerDied) return;
		t += ts;
		glm::vec3 nextPos = NextPos(speed, pos, ts);
		if (!m_tilemap->BoxCollision(nextPos, size))
		{
			pos = nextPos;
		}

		if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			for (float i = 1; i <= 10; i += 2)
			{
				// Convert mouse screen coordinates to world coordinates
				glm::vec2 worldMousePos = m_cam->GetCamera().PixelPosToWorldPos(glm::vec3(Input::GetMousePositionVec2(), 1)) * i;

				// Calculate the direction vector from player to mouse
				glm::vec2 direction = worldMousePos - glm::vec2(pos.x, pos.y);

				// Avoid normalizing a zero vector
				if (glm::length(direction) > 0.0f) {
					glm::vec2 normalizedDirection = glm::normalize(direction);

					ENTGuner* g = &m_Scene->CreateEntity<ENTGuner>("tmp");
					g->dir = glm::vec3(normalizedDirection, 0) * (GetHealthPercent() * 2.501f); // Set gunner direction
					g->Setup(m_texA[1], this);
					g->pos = this->pos; // Set gunner position to player position
					lol->emplace_back(g);
				}
			}
			
		}

		m_cam->m_CameraPosition = moveTowardss(m_cam->m_CameraPosition, nextPos, 9 * ts);
	}
	
	void ENTInfoPlayer::Draw(Timestep& ts)
	{
		t += ts;
		Renderer2D::DrawQuad(glm::vec2(pos.x, pos.y + (sin(t) / 10)), size, m_texA[0], 1);

		if (hasPermission(Key::Red))
		{
			Renderer2D::DrawQuad(glm::vec3(-10, -6, 0) + m_cam->m_CameraPosition, size * 2.0f, m_texA[0], 1);
		}

		if (hasPermission(Key::Blue))
		{
			Renderer2D::DrawQuad(glm::vec3(-8, -6, 0) + m_cam->m_CameraPosition, size * 2.0f, m_texA[0], 1);
		}

		if (hasPermission(Key::Green))
		{
			Renderer2D::DrawQuad(glm::vec3(-6, -6, 0) + m_cam->m_CameraPosition, size * 2.0f, m_texA[0], 1);
		}

		//VX_INFO("{0}, {1}, {2}", m_cam->m_CameraPosition.x, m_cam->m_CameraPosition.y, m_cam->m_CameraPosition.z);
	}
	void ENTInfoPlayer::ImGuiRender()
	{
		ImGuiLink::Begin("Player Info");
		ImGuiLink::Text("Health: %.1f%%", GetHealthPercent() * 200.0f);

		ImGuiLink::End();
	}
	void ENTInfoPlayer::OnDeath()
	{
		hasThePlayerDied = 1;
	}
	void ENTInfoPlayer::Die(glm::vec3 senderPos)
	{

		glm::vec3 dir = pos - senderPos;

		// Handle the zero-length case: senderPos and pos are identical
		if (glm::length(dir) == 0.0f)
		{
			Damage(12);  // No movement or apply minimum damage
			return;
		}

		// Normalize the direction if it's non-zero
		dir = glm::normalize(dir);

		float try_lol = 0.0f;
		

		for (float i = 0.01f; i < 12.0f; i += 0.01f)
		{
			if (m_tilemap->BoxCollision(pos + dir * i, size))
			{
				break;
			}

			try_lol = i;
		}
		Damage((int)try_lol);


		pos += dir * try_lol;
	}

	void ENTInfoPlayer::Setup(Ref<Texture2D> tex, Ref<Texture2D> tex2, ENTEnvStaticTilemap* tilemap, OrthographicCameraController* cam)
	{
		m_cam = cam;
		key = static_cast<uint8_t>(Key::Red) | static_cast<uint8_t>(Key::Green) | static_cast<uint8_t>(Key::Blue);
		m_texA[0] = tex;
		m_texA[1] = tex2;
		m_tilemap = tilemap;
	}
}