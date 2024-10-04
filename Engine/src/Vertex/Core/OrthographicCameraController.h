#pragma once
#include "Vertex/Renderer/OrthographicCamera.h"
#include "Vertex/Core/Timestep.h"

#include "Vertex/Events/ApplicationEvent.h"
#include "Vertex/Events/MouseEvent.h"

namespace Vertex {

	class VERTEX_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false, bool canBeMoved = true);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		float m_ZoomLevel = 6.9f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		
		OrthographicCamera m_Camera;

		bool m_Rotation;
		bool m_canBeMoved;

		
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}
