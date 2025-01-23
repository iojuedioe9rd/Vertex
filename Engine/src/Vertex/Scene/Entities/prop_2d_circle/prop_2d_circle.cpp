#include <vxpch.h>

#include "prop_2d_circle.h"
#include "Vertex/Renderer/Renderer2D.h"
#include "Vertex/Math/Math.h"

namespace Vertex
{
	ENTProp2DCircle::~ENTProp2DCircle()
	{
	}

	void ENTProp2DCircle::Update(Timestep& ts)
	{
	}

	void ENTProp2DCircle::Draw(Timestep& ts)
	{
		
		glm::mat4 mat = Math::ComposeTransform(pos, size, rotation);

		Renderer2D::DrawCircle(mat, colour, Thickness, Fade, GetIntID());
		
	}
}