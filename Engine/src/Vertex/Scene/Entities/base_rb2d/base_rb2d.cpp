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
		
		if (Type != BodyType::Static)
		{
			b2Body* body = (b2Body*)RuntimeBody;
			const auto& position = body->GetPosition();
			pos.x = position.x;
			pos.y = position.y;
			rotation.z = glm::degrees(body->GetAngle());
		}
		
		
		
	}
	void ENTBaseRigidbody2D::Update(Timestep& ts)
	{

		b2Body* body = (b2Body*)RuntimeBody;
		body->SetTransform(b2Vec2(pos.x, pos.y), glm::radians(rotation.z));
		
		
	}
}