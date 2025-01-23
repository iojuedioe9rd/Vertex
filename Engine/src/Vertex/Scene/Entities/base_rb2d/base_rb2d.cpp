#include <vxpch.h>

#include "base_rb2d.h"
#include <box2d/b2_body.h>

namespace Vertex
{
	ENTBaseRigidbody2D::ENTBaseRigidbody2D()
	{
	}

	ENTBaseRigidbody2D::~ENTBaseRigidbody2D()
	{
	}

	void ENTBaseRigidbody2D::PhysUpdate(Timestep& ts)
	{
		b2Body* body = (b2Body*)RuntimeBody;
		const auto& position = body->GetPosition();
		pos.x = position.x;
		pos.y = position.y;
		rotation.z = glm::degrees(body->GetAngle());
	}
}