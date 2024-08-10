#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Vertex {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		VX_PROFILE_FUNCTION();

		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			VX_PROFILE_SCOPE("Renderer Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
		}

		{
			static float rotation = 0.0f;
			rotation += ts * 50.0f;

			VX_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
			Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f)
			{
				for (float x = -5.0f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}
			Renderer2D::EndScene();
			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = true;

		

		if (!dockingEnabled)
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
			ImGuiLink::Image((void*)textureID, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });
			
		}
		
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}

	void EditorLayer::DockSpaceCallback()
	{
		ImGuiLink::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGuiLink::Text("Renderer2D Stats:");
		ImGuiLink::Text("Draw Calls: %d", stats.DrawCalls);
		ImGuiLink::Text("Quads: %d", stats.QuadCount);
		ImGuiLink::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGuiLink::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ 1280, 720 }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });
		ImGuiLink::End();

		ImGuiLink::End();
	}

}

