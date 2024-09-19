#include "env_static_tilemap.h"
#include <algorithm> 
#include "Vertex/Renderer/Renderer2D.h"

namespace Vertex
{
	

	void ENTEnvStaticTilemap::Update(Timestep& ts)
	{
	}
	void ENTEnvStaticTilemap::Draw(Timestep& ts)
	{
		for (Tile t : Tiles)
		{
			Renderer2D::DrawQuad(t.pos, glm::vec2(1, 1), t.colour);
		}
	}

	void ENTEnvStaticTilemap::SetTile(glm::i32vec2 pos, Texture2D* tex, glm::vec4 colour, bool m_override)
	{
		for ( Tile t : Tiles )
		{
			if (m_override && pos == t.pos) {
				t.tex = tex;
				t.colour = colour;
				return;
			}
		}

		Tile t = Tile();
		t.pos = pos;
		t.tex = tex;
		t.colour = colour;
		Tiles.emplace_back(t);

	}
	bool ENTEnvStaticTilemap::RemoveTile(glm::i32vec2 pos)
	{
		for (Tile t : Tiles)
		{
			if (pos == t.pos) {
				auto it = std::find(Tiles.begin(), Tiles.end(),
					t);
				if (it != Tiles.end()) {
					Tiles.erase(it);
				}
				return true;
			}
		}

		return false;
	}
}