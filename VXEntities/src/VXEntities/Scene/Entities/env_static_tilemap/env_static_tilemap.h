#pragma once
#include "../../Entity.h"
#include <Vertex/Renderer/Texture.h>


namespace Vertex {

	class ENTEnvStaticTilemap : public Entity
	{
	public:
		virtual void ImGuiRender() override {};
		class Tile : public Object
		{
		public:
			glm::i32vec2 pos = glm::i32vec2();
			glm::vec4 colour = glm::vec4(1,1,1,1);
			Ref<Texture2D> tex = nullptr;
			bool isCollidable;
		};

		ENTEnvStaticTilemap()
		{

		}
		ENTEnvStaticTilemap(std::string name, Scene* scene) : Entity(name, scene)
		{

		}
		~ENTEnvStaticTilemap() = default;

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;

		void SetTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, bool m_override = true, bool isCollidable = true);
		void AddTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, bool isCollidable = true)
		{
			SetTile(pos, tex, colour, false, isCollidable);
		}
		
		bool RemoveTile(glm::i32vec2 pos);

		void Clear()
		{
			Tiles.clear();
		}

		virtual std::string GetEntName()  override
		{
			return "env_static_tilemap";
		}

		bool BoxCollision(glm::vec3 rectPos, glm::vec2 rectSize);

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);

	private:
		std::vector<Tile> Tiles;
	};


}