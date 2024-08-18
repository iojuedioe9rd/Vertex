#include "TileMap.h"
#include <Vertex/Renderer/Renderer2D.h>

namespace Vertex {

	Tile* TileMap::Add(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, glm::vec2 size)
	{
		Tile* t = new Tile();
		t->pos = pos;
		t->tex = tex;
		t->size = size;
		t->colour = colour;
		t->uuid = get_uuid();
		tiles.push_back(t);
		return t;
	}

	bool TileMap::Remove(Tile* tile)
	{
		for (Tile* t : tiles)
		{
			if (tile->uuid == t->uuid)
			{
				delete tile;
				return true;
			}
		}

		return false;
	}

	void TileMap::Draw()
	{
		for (Tile* t : tiles)
		{
			Renderer2D::DrawQuad(t->pos, t->size, t->tex, 1, t->colour);
		}
	}

	TileMap::TileMap()
	{
	}

	TileMap::~TileMap()
	{
		for (Tile* t : tiles)
		{
			delete t;
		}
		tiles.clear();
	}

}