#include "prop_static_sprite.h"
#include "Vertex/Renderer/Renderer2D.h"

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

		Renderer2D::DrawRotatedQuad(glm::vec2(pos.x, pos.y), glm::vec2(size.x, size.y), rotation.z, colour);
	}
}