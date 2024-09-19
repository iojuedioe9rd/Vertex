#pragma once
#include "../../Entity.h"
#include <Vertex/Renderer/Texture.h>


namespace Vertex {

	class ENTEnvStaticTilemap : public Entity
	{
	public:

		class Tile : public Object
		{
		public:
			glm::i32vec2 pos = glm::i32vec2();
			glm::vec4 colour = glm::vec4(1,1,1,1);
			Texture2D* tex = nullptr;
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

		void SetTile(glm::i32vec2 pos, Texture2D* tex, glm::vec4 colour, bool m_override = true);
		void AddTile(glm::i32vec2 pos, Texture2D* tex, glm::vec4 colour)
		{
			SetTile(pos, tex, colour, false);
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

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);

	private:
		std::vector<Tile> Tiles;
	};


}