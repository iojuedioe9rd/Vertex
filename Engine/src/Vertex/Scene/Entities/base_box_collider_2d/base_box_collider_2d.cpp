#include <vxpch.h>

#include "base_box_collider_2d.h"

namespace Vertex
{
	ENTBaseBoxCollier2D::ENTBaseBoxCollier2D()
	{
	}

	void ENTBaseBoxCollier2D::PhysUpdate(Timestep& ts)
	{
		ENTBaseRigidbody2D::PhysUpdate(ts);
	}

	void ENTBaseBoxCollier2D::Update(Timestep& ts)
	{
		ENTBaseRigidbody2D::Update(ts);
	}

	ENTBaseBoxCollier2D::~ENTBaseBoxCollier2D()
	{
	}

}