#pragma once
#include "../../Entity.h"
#include "../base_rb2d/base_rb2d.h"

namespace Vertex
{
	class VERTEX_API ENTBaseBoxCollier2D : public ENTBaseRigidbody2D
	{
	public:
		
		ENTBaseBoxCollier2D();
		ENTBaseBoxCollier2D(std::string name, Scene* scene) : ENTBaseRigidbody2D(name, scene)
		{

		}

		void SetRuntimeFixture(void* NewRuntimeFixture)
		{
			RuntimeFixture = NewRuntimeFixture;
		}

		~ENTBaseBoxCollier2D();

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		virtual SerializationObject Serialize(bool isInSerializer = false) override
		{
			SerializationObject obj = ENTBaseRigidbody2D::Serialize(isInSerializer);
			obj.Set("Density", SerializationType::Float, Density);
			obj.Set("Friction", SerializationType::Float, Friction);
			obj.Set("Restitution", SerializationType::Float, Restitution);
			obj.Set("RestitutionThreshold", SerializationType::Float, RestitutionThreshold);
			obj.Set("Offset", SerializationType::Vector2, Offset);
			obj.Set("Size", SerializationType::Vector2, Size);
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
		{
			ENTBaseRigidbody2D::DeSerialize(obj, isInSerializer);
			Density = obj.Get<float>("Density", SerializationType::Float);
			Friction = obj.Get<float>("Friction", SerializationType::Float);
			Restitution = obj.Get<float>("Restitution", SerializationType::Float);
			RestitutionThreshold = obj.Get<float>("RestitutionThreshold", SerializationType::Float);
			Offset = obj.Get<glm::vec2>("Offset", SerializationType::Vector2);
			Size = obj.Get<glm::vec2>("Size", SerializationType::Vector2);
			return true;
		}

	protected:
		void* RuntimeFixture = nullptr;
	private:

	};


}