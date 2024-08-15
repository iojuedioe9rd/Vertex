#pragma once

#include <glm/glm.hpp>
#include "Vertex/Core/Base.h"

namespace Vertex {

	class VERTEX_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; m_ViewMatrixDirty = true; }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; m_ViewMatrixDirty = true; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;

		glm::vec2 PixelPosToWorldPos(const glm::vec3& pixelPos);

		// Add a method to set the window size
		void SetWindowSize(float width, float height)
		{
			m_InvWindowWidth = 2.0f / width;
			m_InvWindowHeight = 2.0f / height;
		}

		std::vector<glm::vec2> BatchPixelPosToWorldPos(const std::vector<glm::vec3>& pixelPositions);
	private:
		void RecalculateViewMatrix() const;
	private:
		glm::mat4 m_ProjectionMatrix;
		mutable glm::mat4 m_ViewMatrix; // Marked as mutable because it's modified in const method
		mutable glm::mat4 m_ViewProjectionMatrix;
		mutable glm::mat4 m_InverseViewProjectionMatrix;
		mutable bool m_ViewMatrixDirty = true;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

		// Declare the inverse window dimensions
		float m_InvWindowWidth = 1.0f;
		float m_InvWindowHeight = 1.0f;
	};

}