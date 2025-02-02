#pragma once
#include <Vertex/Scene/Behaviour.h>
#include <Vertex/Scene/Entity.h>

namespace Vertex
{
	class VERTEX_API BEHAVIOURMoveToPosAndBack : public Behaviour
	{
	public:
		BEHAVIOURMoveToPosAndBack(Entity* entity) : Behaviour(entity) 
		{
			m_InitialPosition = m_Entity->pos;
		}
		~BEHAVIOURMoveToPosAndBack() {}

		virtual std::string GetTypeName() override
		{
			return "move_to_pos_and_back";
		}

		virtual void OnUpdate(Timestep& ts) override;


		float speed = 1;

		glm::vec3 gotoPos = glm::vec3(0, 0, 0);

		virtual SerializationObject Serialize() override
		{
			SerializationObject obj = Behaviour::Serialize();
			
			obj.Set("speed", SerializationType::Float, speed);
			obj.Set("gotoPos", SerializationType::Vector3, gotoPos);
			obj.Set("m_InitialPosition", SerializationType::Vector3, m_InitialPosition);
			obj.Set("m_Dir", SerializationType::Bool, m_Dir);

			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj) override
		{
			Behaviour::DeSerialize(obj);

			speed = obj.Get<float>("speed", SerializationType::Float);
			gotoPos = obj.Get<glm::vec3>("gotoPos", SerializationType::Vector3);
			m_InitialPosition = obj.Get<glm::vec3>("m_InitialPosition", SerializationType::Vector3);
			m_Dir = obj.Get<bool>("m_Dir", SerializationType::Bool);


			return true;
		}
		
	private:
		glm::vec3 m_InitialPosition;
		bool m_Dir = false;

	};

	
	
}
