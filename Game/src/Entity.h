#pragma once

#include <glm/glm.hpp>
#include <Vertex/Renderer/Texture.h>
#include <Vertex/Core/Timestep.h>

namespace Vertex {

	class Entity {
	public: 
		Entity(glm::vec2 pos, Texture* tex);
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
		virtual Texture* getTex() = 0;
		virtual void update(Timestep ts) = 0;
		virtual void draw(Timestep ts) = 0;

	private:
		glm::vec2 m_pos;
		float m_angle = 0;
		glm::vec2 m_scale = glm::vec2(1, 1);
		Texture* m_tex;
	};

}
