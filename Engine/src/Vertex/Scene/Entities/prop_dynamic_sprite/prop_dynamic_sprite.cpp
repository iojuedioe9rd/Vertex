#include <vxpch.h>

#include "prop_dynamic_sprite.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "Vertex/Math/Math.h"

namespace Vertex
{
	ENTPropDynamicSprite::~ENTPropDynamicSprite()
	{
	}
	void ENTPropDynamicSprite::Update(Timestep& ts)
	{
		// Do nothing but if we don't define it won't work ╰（‵□′）╯
	}

	void ENTPropDynamicSprite::Draw(Timestep& ts)
	{
		if (Type != ENTBaseRigidbody2D::BodyType::Dynamic)
		{
			Type = ENTBaseRigidbody2D::BodyType::Dynamic;
		}
		int id = 0;
		for (char c : GetID())
		{
			id += c;
		}
		glm::mat4 mat = Math::ComposeTransform(pos, size, rotation);

		if (texture)
		{
			Renderer2D::DrawQuad(mat, texture, tilingFactor, colour, GetIntID());
		}
		else
		{
			Renderer2D::DrawQuad(mat, colour, GetIntID());
		}

	}

	ENT_REGISTER_ENTITY(ENTPropDynamicSprite, prop_dynamic_sprite);
}
