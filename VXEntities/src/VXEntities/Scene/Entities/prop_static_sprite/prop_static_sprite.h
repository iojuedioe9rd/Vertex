#pragma once
#include "../../Entity.h"
#include "Vertex/Renderer/Texture.h"

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
		virtual void EventH(Event& e) override
		{

		}

		glm::vec4 colour = glm::vec4(1,1,1,1);
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

	private:

	};

	
}