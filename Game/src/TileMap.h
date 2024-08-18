#pragma once

#include "glm/glm.hpp"
#include <Vertex/Renderer/Texture.h>
#include "UUID.h"
#include <vector>

namespace Vertex {

	struct Tile 
	{
		glm::i32vec2 pos;
		glm::vec4 colour;
		glm::vec2 size;
		Ref<Texture2D> tex;
		UUID uuid;
	};

	class TileMap
	{
	public:
		Tile* Add(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour = glm::vec4(1, 1, 1, 1), glm::vec2 size = glm::vec2(1, 1));
		bool Remove(Tile* tile);

		void Draw();

		TileMap();
		~TileMap();

	private:
		std::vector<Tile*> tiles;
	};


}




