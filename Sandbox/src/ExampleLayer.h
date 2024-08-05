#pragma once
#include "Vertex.h"
#include <glm/gtc/type_ptr.hpp>
#include <Vertex/Renderer/Texture.h>


class ExampleLayer : public Vertex::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	void OnEvent(Vertex::Event& e) override;

	virtual void OnImGuiRender() override
	{
		ImGuiLink::Begin("Settings");
		ImGuiLink::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGuiLink::ColorEdit3("Cam Pos", glm::value_ptr(m_CameraPosition));
		ImGuiLink::End();
	}

private:
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	Ref<Shader> m_FlatColorShader;
	Ref<VertexArray> m_VertexArray;

	Ref<Shader> m_TextureShader;
	Ref<VertexArray> m_SquareVA;

	Ref<Texture2D> m_Texture, m_PlayerTexture;

	OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition = {0.5f, 0.5f, 0.0f};
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
};

