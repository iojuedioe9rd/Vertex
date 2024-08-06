#include "Sandbox2D.h"
#include "Vertex/Renderer/Renderer2D.h"

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
	m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
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

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
	Renderer2D::EndScene();
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