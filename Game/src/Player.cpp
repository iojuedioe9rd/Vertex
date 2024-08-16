#include "Player.h"
#include <Vertex/Renderer/Renderer2D.h>
#include <Vertex/Core/Logger.h>

namespace Vertex {

	Player::Player() : Entity(glm::vec2(0, 0), Texture2D::Create("assets/textures/Player.png"))
	{
		m_scale = glm::vec2(1, 1);
	}

	Ref<Texture2D> Player::getTex()
	{
		return m_tex;
	}

	void Player::update(Timestep ts)
	{
		VX_INFO("{0}", ts.operator float());
		m_pos += glm::vec2(1, 0) * ts.operator float();
	}

	void Player::draw(Timestep ts)
	{
		
		Renderer2D::DrawRotatedQuad(m_pos, m_scale, m_angle, m_tex);
	}

	uint16_t Player::getEntityType()
	{
		return ENTITY_TYPE_PLAYER;
	}

}