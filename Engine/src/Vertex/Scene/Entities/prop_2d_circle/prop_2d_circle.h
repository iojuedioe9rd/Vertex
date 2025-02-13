#pragma once
#include "../../Entity.h"
#include "../base_box_collider_2d/base_box_collider_2d.h"
#include "Vertex/Renderer/Texture.h"

namespace Vertex {

	class VERTEX_API ENTProp2DCircle : public ENTBaseBoxCollier2D
	{
	public:
		ENTProp2DCircle()
		{

		}
		ENTProp2DCircle(std::string name, Scene* scene) : ENTBaseBoxCollier2D(name, scene)
		{

		}
		~ENTProp2DCircle();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual std::string GetEntName()  override
		{
			return "prop_2d_circle";
		}
		virtual void EventH(Event& e) override
		{

		}

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);
		float Thickness = 1.0f;
		float Fade = 0.005f;

		virtual SerializationObject Serialize(bool isInSerializer = false) override
		{
			SerializationObject obj = ENTBaseBoxCollier2D::Serialize(isInSerializer);
			obj.Set("Colour", SerializationType::Vector4, colour);
			obj.Set("Thickness", SerializationType::Float, Thickness);
			obj.Set("Fade", SerializationType::Float, Fade);
			

			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
		{
			ENTBaseBoxCollier2D::DeSerialize(obj, isInSerializer);
			colour = obj.Get<glm::vec4>("Colour", SerializationType::Vector4);
			Thickness = obj.Get<float>("Thickness", SerializationType::Float);
			Fade = obj.Get<float>("Fade", SerializationType::Float);

			return true;
		}
	private:

	};


}