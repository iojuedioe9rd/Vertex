#include <vxpch.h>

#include "prop_text.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "Vertex/Math/Math.h"

namespace Vertex
{
	ENTPropText3D::~ENTPropText3D()
	{
	}

	ENTPropText2D::~ENTPropText2D()
	{
	}

	void ENTPropText::Update(Timestep& ts)
	{
		rotation.x += 25 * ts;
		rotation.y += 15 * ts;
	}

	void ENTPropText::Draw(Timestep& ts)
	{
	}

	ENTPropText::~ENTPropText()
	{
	}

	

	void ENTPropText2D::Draw(Timestep& ts)
	{
		glm::mat4 mat = Math::ComposeTransform(pos, size, rotation);

		Renderer2D::DrawString(text, font, mat, textParams, GetIntID());

	}

	ENT_REGISTER_ENTITY(ENTPropText2D, prop_text_2d);
	
}