#pragma once
#include "../../Entity.h"
#include <Vertex/Renderer/Texture.h>


namespace Vertex {

	class VERTEX_API ENTEnvStaticTilemap : public Entity
	{
	public:

		class VERTEX_API Tile : public Object
		{
		public:
			glm::i32vec2 pos = glm::i32vec2();
			glm::vec4 colour = glm::vec4(1,1,1,1);
			Ref<Texture2D> tex = nullptr;
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

		void SetTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, bool m_override = true);
		void AddTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour)
		{
			SetTile(pos, tex, colour, false);
		}
		virtual void EventH(Event& e) override
		{

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

		std::vector<Tile> GetTiles()
		{
			return Tiles;
		}

	private:
		std::vector<Tile> Tiles;
	};


}