#include "prop_key_door.h"
#include <Vertex/Renderer/Renderer2D.h>
#include <glm/ext/matrix_transform.hpp>

namespace Vertex
{
	ENTPropKeyDoor::~ENTPropKeyDoor()
	{
		// Destructor is empty, but if you allocate resources in the future,
		// make sure to clean them up here.
	}

	void ENTPropKeyDoor::Update(Timestep& ts)
	{
		// No update logic here yet, but if the door has behavior (e.g., opening/closing),
		// this is where you'd handle it.
	}

    void ENTPropKeyDoor::Draw(Timestep& ts)
    {
        if (!m_isLocked)
            retASMName;
        float rotationAngle = rotation.z; // Use the z component for 2D rotation

        // Base position of the door
        glm::vec2 drawPos = pos;

        // Offsets for the door
        glm::vec2 offset1 = glm::vec2(1.5f, 0.0f); // Adjusted offset for the first quad
        glm::vec2 offset2 = glm::vec2(2.0f, 0.0f); // Adjusted offset for the second quad

        // Calculate the rotation in radians
        float radians = glm::radians(rotationAngle);

        // Rotate offsets manually
        glm::vec2 rotatedOffset1 = {
            offset1.x * cos(radians) - offset1.y * sin(radians),
            offset1.x * sin(radians) + offset1.y * cos(radians)
        };

        glm::vec2 rotatedOffset2 = {
            offset2.x * cos(radians) - offset2.y * sin(radians),
            offset2.x * sin(radians) + offset2.y * cos(radians)
        };

        if (rotationAngle == -180)
        {
            drawPos += glm::vec2(3 / 2, 0);
        }

        if (rotationAngle == 90)
        {
            drawPos -= glm::vec2(0, 1);
        }

        switch (m_key)
        {
        case ENTInfoPlayer::Key::Red:
            // Draw the first door quad with its rotated offset
            Renderer2D::DrawRotatedQuad(drawPos, glm::vec2(3.0f, 0.5f), rotationAngle, (*m_textures)[0], 1, glm::vec4(1, 0, 0, 1));
            // Draw the second door quad with its own rotated offset
           
            break;
        case ENTInfoPlayer::Key::Green:
            Renderer2D::DrawRotatedQuad(drawPos, glm::vec2(3.0f, 0.5f), rotationAngle, (*m_textures)[0], 1, glm::vec4(0, 1, 0, 1));
            break;
        case ENTInfoPlayer::Key::Blue:
            Renderer2D::DrawRotatedQuad(drawPos, glm::vec2(3.0f, 0.5f), rotationAngle, (*m_textures)[0], 1, glm::vec4(0, 0, 1, 1));
            break;
        default:
            break;
        }
    }



	void ENTPropKeyDoor::Setup(std::array<Ref<Texture2D>, 1>* textures, ENTInfoPlayer::Key key)
	{
		// Assign the texture array and key to the door
		m_textures = textures;
		m_key = key;
	}
}
