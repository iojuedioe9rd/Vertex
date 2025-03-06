#pragma once
#include <string>
#include "../../Entity.h"
#include "Vertex/Object/SerializationObject.h"
#include <Vertex/Scene/Entities/base_box_collider_2d/base_box_collider_2d.h>

namespace Vertex
{
	class VERTEX_API ENTTriggerLava : public ENTBaseBoxCollier2D
	{
	public:
		ENTTriggerLava()
		{

		}
		ENTTriggerLava(std::string name, Scene* scene) : ENTBaseBoxCollier2D(name, scene)
		{
			this->Size = glm::vec2(size.x / 2, size.y / 2);
		}
		~ENTTriggerLava() = default;

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual void EventH(Event& e) override
		{

		}


		virtual std::string GetEntName()  override
		{
			return "trigger_lava";
		}

		virtual void BeginContact(Entity* entity) override;
		virtual void PhysUpdate(Timestep& ts) override;
		virtual void EndContact(Entity* entity) override;

		virtual SerializationObject Serialize(bool isInSerializer = false) override
		{
			SerializationObject obj = ENTBaseBoxCollier2D::Serialize(isInSerializer);
			
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
		{
			ENTBaseBoxCollier2D::DeSerialize(obj, isInSerializer);
			
			return true;
		}
	private:

	};
}

