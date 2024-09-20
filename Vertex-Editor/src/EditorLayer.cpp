#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entities/json_static_tilemap/json_static_tilemap.h"


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

		m_CameraController.GetCamera().SetWindowSize(1280, 720);
		m_CameraController.GetCamera().SetPosition(glm::vec3(0,0,0));



		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = VXEntities_MakeOrGetScene("ActiveScene");
		// Entity

		auto& square = m_ActiveScene->CreateEntity<ENTPropStaticSprite>("Green Square");
		
		m_ActiveScene->CreateEntity<ENTJsonStaticTilemap>("JsonStaticTilemap").Setup(&m_ActiveScene->CreateEntity<ENTEnvStaticTilemap>("Tilemap"));
		
		
		square.colour = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
		square.SetIsVisible(true);

		m_SquareEntity = square;
		m_SquareEntity.colour = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
	}

	float t = 0.0f;

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		
		
		
		t += ts;
		
		m_ViewportFocused = true;

		VX_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		//m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());


		Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		// Update scene
		m_ActiveScene->OnUpdate(ts);
		m_SquareEntity.pos = glm::vec3(0, sinf(t) * 100, 0);
		VX_INFO("{0}", sinf(t));
		
		

		Renderer2D::EndScene();

		//m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = 0;

		ImGuiLink::Docking(dockingEnabled, [this] { DockSpaceCallback(); });

		if (1)
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

			ImGuiLink::End();
			
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

		if (true)
		{
			ImGuiLink::Separator();
			
			ImGuiLink::Text("%s", m_SquareEntity.name());

			m_SquareEntity.colour = m_SquareColor;
			ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
			ImGuiLink::Separator();

		}
		ImGuiLink::End();

		ImGuiLink::PushStyleVar(ImGuiLink::ImGuiStyleVar_WindowPadding, glm::vec2{ 0, 0 });
		ImGuiLink::Begin("Viewport");

		m_ViewportFocused = ImGuiLink::IsWindowFocused();
		m_ViewportHovered = ImGuiLink::IsWindowHovered();

		glm::vec2 viewportPanelSize = ImGuiLink::GetContentRegionAvail();

		if (m_ViewportSize != viewportPanelSize)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });

		ImGuiLink::End();
		ImGuiLink::PopStyleVar();

		ImGuiLink::Begin("Hi");
		
		ImGuiLink::End();

		
	}

}

