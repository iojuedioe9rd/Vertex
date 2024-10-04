#pragma once
#include "VXEntities.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include "../base_health_system/base_health_system.h"
#include "Vertex/Renderer/Renderer2D.h"

namespace Vertex {

	class ENTGuner;
	

	class ENTInfoPlayer : public ENTBaseHealthSystem<int, 0>
	{
	public:

		enum class Key : uint8_t
		{
			Red = BIT(0),
			Green = BIT(1),
			Blue = BIT(2)
		};
		
		Ref<std::vector<ENTGuner*>> GetGuner();

		float speed = 5;

		ENTInfoPlayer()
		{

		}
		void pass();
		ENTInfoPlayer(std::string name, Scene* scene) : ENTBaseHealthSystem(name, scene)
		{
			pass();
			Init(100, 253);
			
		}
		~ENTInfoPlayer();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual void ImGuiRender() override;
		float t = 0.0f;
		virtual std::string GetEntName() override
		{
			return "info_player";
		}

		bool AddKey(Key keyToAdd) {
			key |= static_cast<uint8_t>(keyToAdd);

		}

		bool hasPermission(Key perm) {
			return (static_cast<uint8_t>(key) & static_cast<uint8_t>(perm)) != 0;
		}

		void RemoveKey(Key keyToRemove)
		{
			key &= ~static_cast<uint8_t>(keyToRemove);  // Remove the specified flag
		};
		virtual void OnDeath() override;
		void Die(glm::vec3 senderPos);

		void Setup(Ref<Texture2D> tex, Ref<Texture2D> tex2, ENTEnvStaticTilemap* tilemap, OrthographicCameraController* cam);

	private:
		bool hasThePlayerDied = 0;
		uint8_t key;

		OrthographicCameraController* m_cam;
		std::array<Ref<Texture2D>, 2> m_texA = std::array<Ref<Texture2D>, 2>();
		
		ENTEnvStaticTilemap* m_tilemap;
		bool isSetup;

	};

	class ENTGuner : public Entity
	{
	public:
		virtual void ImGuiRender() override {};
		Ref<Texture2D> m_tex;
		virtual std::string GetEntName() override
		{
			return "info_player_guner";
		}
		ENTGuner()
		{
			size = glm::vec3(1, 1, 1);
		}
		

		ENTGuner(std::string name, Scene* scene) : Entity(name, scene)
		{
			
		}
		~ENTGuner()
		{
			

			m_player->GetGuner();
		}
		ENTInfoPlayer* m_player;
		void Setup(Ref<Texture2D> tex, ENTInfoPlayer* player)
		{
			m_tex = tex;
			m_player = player;
		}

		glm::vec3 dir;
		float timer = 0.0f;
		virtual void Update(Timestep& ts) override
		{

			// Ensure the direction is normalized before applying movement
			if (glm::length(dir) > 0.0f) {
				dir = glm::normalize(dir);
			}

			// Update the position based on direction, speed, and elapsed time
			pos += dir * 10.0f * ts.GetSeconds();

			// Increment the timer with the time step
			timer += ts;

			// Correct the size reduction using the seconds from timestep
			size -= glm::vec3(ts.GetSeconds(), ts.GetSeconds(), ts.GetSeconds()) / 5.0f;

			// Optional: add checks to prevent size from becoming negative or invalid
			size = glm::max(size, glm::vec3(0.0f)); // Ensure size does not go below zero

			if (size == glm::vec3(0.0f))
			{
				m_Scene->RemoveEntity(*this);
			}
		}
		virtual void Draw(Timestep& ts)
		{

			Renderer2D::DrawQuad(glm::vec2(pos.x, pos.y), glm::vec2(size.x, size.y), m_tex, 1);
		}


	};
}
