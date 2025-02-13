#pragma once
#include "../../Entity.h"

namespace Vertex
{
	class VERTEX_API ENTBaseRigidbody2D : public Entity
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
		virtual void Update(Timestep& ts) override;
		
		virtual SerializationObject Serialize(bool isInSerializer = 0) override
		{
			SerializationObject obj = Entity::Serialize(isInSerializer);
			obj.Set("RB2DType", SerializationType::Int, (int)Type);
			obj.Set("RB2DFixedRotation", SerializationType::Bool, FixedRotation);
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = 0) override
		{
			Entity::DeSerialize(obj);
			Type = (BodyType)obj.Get<int>("RB2DType", SerializationType::Int);
			FixedRotation = obj.Get<bool>("RB2DFixedRotation", SerializationType::Bool);
			return true;
		}

	protected:
		void* RuntimeBody = nullptr;
	private:

	};

	
}