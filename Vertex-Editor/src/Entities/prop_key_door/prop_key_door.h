#pragma once

#include <VXEntities.h>
#include <array>
#include "../info_player/info_player.h"

namespace Vertex
{

#define ENTPropKeyDoor_TextureType std::array<::Vertex::Texture2D, 1>

	class ENTPropKeyDoor : public Entity
	{
	public:
		ENTPropKeyDoor()
		{

		}
		ENTPropKeyDoor(std::string name, Scene* scene) : Entity(name, scene)
		{

		}
		~ENTPropKeyDoor();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual std::string GetEntName()  override
		{
			return "prop_static_sprite";
		}

		void UnLock()
		{
			m_isLocked = false;
		}

		void Lock()
		{
			m_isLocked = true;
		}

		bool IsLocked() const
		{
			retASMName m_isLocked;
		}

		virtual void ImGuiRender() override 
		{
			
		};
		glm::vec3 offset;
		ENTInfoPlayer::Key m_key = ENTInfoPlayer::Key::Red;

		void Setup(std::array<Ref<Texture2D>, 1>* textures, ENTInfoPlayer::Key key = ENTInfoPlayer::Key::Red);
		bool CanPlayerUnLock = true;
	private:
		bool m_isLocked = true;

		std::array<Ref<Texture2D>, 1>* m_textures;
		
	};
}