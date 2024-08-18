#pragma once

#include <glm/glm.hpp>
#include <Vertex/Renderer/Texture.h>
#include <Vertex/Core/Timestep.h>
#include "UUID.h"

#define ENTITY_TYPE_PLAYER BIT(0)
#define ENTITY_TYPE_EMEMY  BIT(1)

namespace Vertex {

	
	class Entity {
	public: 
		Entity(glm::vec2 pos, Ref<Texture2D> tex);
		Entity(glm::vec2 pos);
		Entity();
		glm::vec2& getPos()
		{
			return m_pos;
		}
		float getAngle() const
		{
			return m_angle;
		}
		glm::vec2& getScale() 
		{
			return m_scale;
		}

		void setPos(float x, float y);
		void setScale(float w, float h);
		void setAngle(float angle);
		virtual Ref<Texture2D> getTex() = 0;
		virtual void update(Timestep ts) = 0;
		virtual void draw(Timestep ts) = 0;

		virtual uint16_t getEntityType() = 0;

		bool operator==(const Entity& other) const
		{
			return id == other.id;
		}

	protected:
		UUID id;
		glm::vec2 m_pos;
		float m_angle = 0;
		glm::vec2 m_scale = glm::vec2(1, 1);
		Ref<Texture2D> m_tex;
	};

}
