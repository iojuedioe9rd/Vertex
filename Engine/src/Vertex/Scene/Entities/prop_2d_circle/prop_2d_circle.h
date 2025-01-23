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
	private:

	};


}