#pragma once

#include "Entity.h"



namespace Vertex {

	struct EnemyS
	{
		int health;
		int healthMax;
		float speed;
		float speedMax;

		float sizeMod;
		float sizeModMax;
	};


	class Enemy : public Entity
	{
	public:
		Enemy(int health, float speed, float sizeMod);
		~Enemy();


		virtual Ref<Texture2D> getTex() override;
		virtual void update(Timestep ts) override;
		virtual void draw(Timestep ts) override;

		virtual uint16_t getEntityType() override;

		void Damage(int amount);
		int GetHealth();


	private:
		EnemyS* meeeeeeeee;
	};

}

