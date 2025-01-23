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
	private:
		
	};
}

