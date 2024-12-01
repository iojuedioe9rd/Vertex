#include "Sandbox2D.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <Vertex/ImGui/ImGuiLink.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Vertex/Audio/AudioManager.h>

using namespace Vertex;


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
	//audio = AudioManager::GetAudioFromFileName("assets/music/idk.wav", true);
	//audio->Play();

	//FramebufferSpecification fbSpec;
	//fbSpec.Width = 1280;
	//fbSpec.Height = 720;
	//m_Framebuffer = Framebuffer::Create(fbSpec);

	
}

void Sandbox2D::OnDetach()
{
	
}

float t = 0.1f;

void Sandbox2D::OnUpdate(Timestep ts)
{
	

	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	
	
		

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	
	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
	Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1000.0f, 1000.0f }, m_CheckerboardTexture, 1000.0f);
	Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

	for (float y = -10.0f; y < 10.0f; y += 0.5f)
	{
		for (float x = -10.0f; x < 10.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 10.0f) / 20.0f, 0.4f, (y + 10.0f) / 20.0f, 0.7f };
			Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}
	Renderer2D::EndScene();
	
}

void Sandbox2D::OnImGuiRender()
{



	ImGuiLink::Begin("Settings");

	auto stats = Renderer2D::GetStats();
	ImGuiLink::Text("Renderer2D Stats:");
	ImGuiLink::Text("Draw Calls: %d", stats.DrawCalls);
	ImGuiLink::Text("Quads: %d", stats.QuadCount);
	ImGuiLink::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGuiLink::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGuiLink::Image((void*)textureID, glm::vec2{ 100, 100 });
	ImGuiLink::End();

}

void Sandbox2D::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

