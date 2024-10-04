#include "vxpch.h"
#include "OrthographicCamera.h"
#include "Vertex/Core/Application.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Vertex {

    float Remap(float source, float sourceFrom, float sourceTo, float targetFrom, float targetTo)
    {
        return targetFrom + (source - sourceFrom) * (targetTo - targetFrom) / (sourceTo - sourceFrom);
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        m_ViewMatrixDirty = true;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        m_InverseViewProjectionMatrix = glm::inverse(m_ViewProjectionMatrix);
        
    }

    const glm::mat4& OrthographicCamera::GetViewMatrix() const
    {
        if (m_ViewMatrixDirty)
        {
            RecalculateViewMatrix();
        }
        return m_ViewMatrix;
    }

    const glm::mat4& OrthographicCamera::GetViewProjectionMatrix() const
    {
        if (m_ViewMatrixDirty)
        {
            RecalculateViewMatrix();
        }
        return m_ViewProjectionMatrix;
    }

    

    void OrthographicCamera::RecalculateViewMatrix() const
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

        m_InverseViewProjectionMatrix = glm::inverse(m_ViewProjectionMatrix);  // Update the inverse
        m_ViewMatrixDirty = false; // Clear the dirty flag
    }


    std::vector<glm::vec2> OrthographicCamera::BatchPixelPosToWorldPos(const std::vector<glm::vec3>& pixelPositions)
    {
        std::vector<glm::vec2> worldPositions(pixelPositions.size());

        std::transform(
            std::execution::par_unseq, // Parallel and vectorized execution
            pixelPositions.begin(), pixelPositions.end(), worldPositions.begin(),
            [this](const glm::vec3& pixelPos) {
                glm::vec4 ndcPos;
                ndcPos.x = pixelPos.x * m_InvWindowWidth - 1.0f;
                ndcPos.y = 1.0f - pixelPos.y * m_InvWindowHeight;
                ndcPos.z = pixelPos.z;
                ndcPos.w = 1.0f;

                glm::vec4 worldPos = m_InverseViewProjectionMatrix * ndcPos;
                return glm::vec2(worldPos.x / worldPos.w, worldPos.y / worldPos.w);
            }
        );

        return worldPositions;
    }

	glm::vec2 OrthographicCamera::PixelPosToWorldPos(const glm::vec3& pixelPos)
	{
        glm::vec3 newPixelPos = pixelPos;
        

        glm::vec4 ndcPos;
        ndcPos.x = pixelPos.x * m_InvWindowWidth - 1.0f;
        ndcPos.y = 1.0f - pixelPos.y * m_InvWindowHeight;
        ndcPos.z = pixelPos.z;
        ndcPos.w = 1.0f;

        glm::vec4 worldPos = m_InverseViewProjectionMatrix * ndcPos;
        return glm::vec2(worldPos.x / worldPos.w, worldPos.y / worldPos.w);
	}

	

	

}