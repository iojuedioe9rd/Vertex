#pragma once
#include "Entity.h"
#include <Vertex/Audio/Audio.h>


namespace Vertex {

	class Player : public Entity
	{
	public:
		Player();

		Audio* oww;

		virtual Ref<Texture2D> getTex() override;
		virtual void update(Timestep ts) override;
		virtual void draw(Timestep ts) override;

		virtual uint16_t getEntityType() override;
	};

}



