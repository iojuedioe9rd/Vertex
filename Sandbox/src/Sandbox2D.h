#pragma once
#include <Vertex.h>
#include <Vertex/Core/OrthographicCameraController.h>
#include "glm/glm.hpp"

class Sandbox2D : public Vertex::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Vertex::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vertex::Event& e) override;
private:
	Vertex::OrthographicCameraController m_CameraController;

	// Temp
	Vertex::Ref<Vertex::VertexArray> m_SquareVA;
	Vertex::Ref<Vertex::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};