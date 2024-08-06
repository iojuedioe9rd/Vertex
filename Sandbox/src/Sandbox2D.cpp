#include "Sandbox2D.h"
#include <Vertex/ImGui/ImGuiLink.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Vertex;


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA.reset(VertexArray::Create());

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Ref<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());

	(m_FlatColorShader)->Bind();
	(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGuiLink::Begin("Settings");
	ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGuiLink::End();
}

void Sandbox2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}