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
			Renderer2D::DrawQuad(t.pos, glm::vec2(1, 1), t.tex, 1, t.colour);
		}
	}

	void ENTEnvStaticTilemap::SetTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, bool m_override, bool isCollidable)
	{
		for ( Tile t : Tiles )
		{
			if (m_override && pos == t.pos) {
				t.tex = tex;
				t.colour = colour;
				t.isCollidable = isCollidable;
				return;
			}
		}

		Tile t = Tile();
		t.pos = pos;
		t.tex = tex;
		t.isCollidable = isCollidable;
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
	bool ENTEnvStaticTilemap::BoxCollision(glm::vec3 rectPos, glm::vec2 rectSize)
	{
		for (auto& i : Tiles)
		{
			if (!i.isCollidable) continue;
			glm::vec4 rect1 = glm::vec4(rectPos.x, rectPos.y, rectPos.z, rectSize.x);
			glm::vec4 rect2 = glm::vec4(i.pos.x, i.pos.y, 1, 1);


			if (
				rect1.x < rect2.x + rect2.w &&
				rect1.x + rect1.w > rect2.x &&
				rect1.y < rect2.y + rectSize.y &&
				rect1.y + rectSize.y > rect2.y
				) {
				return true;

			}
			
		}

		return false;
	}
}