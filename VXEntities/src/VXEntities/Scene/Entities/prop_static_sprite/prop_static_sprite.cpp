#include "prop_static_sprite.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "Vertex/Math/Math.h"

namespace Vertex
{
	ENTPropStaticSprite::~ENTPropStaticSprite()
	{
	}
	void ENTPropStaticSprite::Update(Timestep& ts)
	{
		// Do nothing but if we don't define it won't work ╰（‵□′）╯
	}

	void ENTPropStaticSprite::Draw(Timestep& ts)
	{
		int id = 0;
		for (char c : GetID())
		{
			id += c;
		}
		glm::mat4 mat = Math::ComposeTransform(pos, size, rotation);

		if (texture)
		{
			Renderer2D::DrawMatrix(mat, texture, tilingFactor, colour, id);
		}
		else
		{
			Renderer2D::DrawMatrix(mat, colour, id);
		}
		
	}
}