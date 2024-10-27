#pragma once
#include "../../Entity.h"
#include "../base_box_collider_2d/base_box_collider_2d.h"
#include "Vertex/Renderer/Texture.h"

namespace Vertex {

	class ENTPropDynamicSprite : public ENTBaseBoxCollier2D
	{
	public:
		ENTPropDynamicSprite()
		{

		}
		ENTPropDynamicSprite(std::string name, Scene* scene) : ENTBaseBoxCollier2D(name, scene)
		{

		}
		~ENTPropDynamicSprite();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual std::string GetEntName()  override
		{
			return "prop_dynamic_sprite";
		}
		virtual void EventH(Event& e) override
		{

		}

		glm::vec4 colour = glm::vec4(1, 1, 1, 1);
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

	private:

	};


}