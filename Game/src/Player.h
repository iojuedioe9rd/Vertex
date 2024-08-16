#pragma once
#include "Entity.h"


namespace Vertex {

	class Player : public Entity
	{
	public:
		Player();


		virtual Ref<Texture2D> getTex() override;
		virtual void update(Timestep ts) override;
		virtual void draw(Timestep ts) override;

		virtual uint16_t getEntityType() override;
	};

}



