#pragma once
#include "../../Entity.h"

namespace Vertex {

	class ENTPropStaticSprite : public Entity
	{
	public:
		ENTPropStaticSprite()
		{

		}
		ENTPropStaticSprite(std::string name, Scene* scene) : Entity(name, scene)
		{
			
		}
		~ENTPropStaticSprite();

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual std::string GetEntName()  override
		{
			return "prop_static_sprite";
		}

		virtual void ImGuiRender() override {};

		glm::vec4 colour = glm::vec4(1,1,1,1);

	private:

	};

	
}