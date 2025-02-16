#pragma once
#include "Vertex/Scene/Behaviour.h"
#include "Vertex/Scene/Entity.h"

namespace Vertex
{
	class BEHAVIOURFollowENT : public Behaviour
	{
	public:

		BEHAVIOURFollowENT(Entity* entity) : Behaviour(entity)
		{

		}



		UUID* entUUID = nullptr;
		Entity* ent = nullptr;

		float speed = 5.0f;

		virtual std::string GetTypeName() override
		{
			return "follow_ent";
		}

		void OnUpdate(Timestep& ts) override;

		virtual SerializationObject Serialize(bool isInSerializer = false) override
		{
			SerializationObject serializationObject = Behaviour::Serialize(isInSerializer);

			serializationObject.Set("speed", SerializationType::Float, speed);
			if (ent)
			{
				serializationObject.Set("entUUID", SerializationType::String, ent->GetID());
			}
			

			return serializationObject;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
		{
			Behaviour::DeSerialize(obj, isInSerializer);

			speed = obj.Get<float>("speed", SerializationType::Float);
			if (obj.Contains("entUUID"))
			{
				this->entUUID = &obj.Get<UUID>("entUUID", SerializationType::String);
				VX_CORE_INFO("{}", entUUID->c_str());
			}
			
		}

		virtual void ImGuiDrawProperties() override;

		virtual void PostDeSerialize() override
		{
			for (Entity* ent: *m_Entity->GetScene())
			{
				if (ent->GetID() == *entUUID)
				{
					this->ent = ent;
				}
			}
		}
	};
}