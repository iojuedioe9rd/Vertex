#pragma once
#include "../../Entity.h"

namespace Vertex
{
	class ENTBaseRigidbody2D : public Entity
	{
	public:
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		ENTBaseRigidbody2D();
		ENTBaseRigidbody2D(std::string name, Scene* scene) : Entity(name, scene)
		{
			
		}

		void SetRuntimeBody(void* NewRuntimeBody)
		{
			RuntimeBody = NewRuntimeBody;
		}

		~ENTBaseRigidbody2D();

		virtual void PhysUpdate(Timestep& ts) override;
		

	protected:
		void* RuntimeBody = nullptr;
	private:

	};

	
}