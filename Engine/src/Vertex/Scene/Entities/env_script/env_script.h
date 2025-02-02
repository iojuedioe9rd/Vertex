#pragma once
#include <string>
#include "../../Entity.h"
#include "Vertex/Object/SerializationObject.h"

namespace Vertex
{
	class VERTEX_API ENTEnvScript : public Entity
	{
	public:
		ENTEnvScript()
		{

		}
		ENTEnvScript(std::string name, Scene* scene) : Entity(name, scene)
		{

		}
		~ENTEnvScript() = default;

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual void EventH(Event& e) override
		{

		}

		virtual void PhysUpdate(Timestep& ts) override;

		virtual std::string GetEntName()  override
		{
			return "env_script";
		}
		bool useRB2D = false;
		bool called = 0;
		std::string classname;
		SerializationObject obj;

		virtual SerializationObject Serialize() override
		{
			SerializationObject obj = Entity::Serialize();
			obj.Set("Use Rigidbody 2D", SerializationType::Bool, useRB2D);
			obj.Set("Classname", SerializationType::String, classname);
			obj.Set("Script Object", SerializationType::SerializationObject, this->obj);
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj) override
		{
			Entity::DeSerialize(obj);
			useRB2D = obj.Get<bool>("Use Rigidbody 2D", SerializationType::Bool);
			classname = obj.Get<std::string>("Classname", SerializationType::String);

			if (obj.Contains("Script Object"))
			{
				this->obj = obj.Get<SerializationObject>("Script Object", SerializationType::SerializationObject);
			}

			return true;
		}
	private:
		
	};
}

