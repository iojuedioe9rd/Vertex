#pragma once
#include "Vertex.h"
#include <glm/gtc/type_ptr.hpp>


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
		ImGuiLink::End();
	}

private:
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	Ref<Shader> m_Shader;
	Ref<VertexArray> m_VertexArray;

	Ref<Shader> m_BlueShader;
	Ref<VertexArray> m_SquareVA;
	OrthographicCamera m_Camera;
};

