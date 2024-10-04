#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entities/json_static_tilemap/json_static_tilemap.h"
#include <gl/GL.h>
#include <Vertex/Renderer/RenderCommand.h>


namespace Vertex {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, 0, 0), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		

		m_CameraController.GetCamera().SetWindowSize(1280, 720);
		m_CameraController.GetCamera().SetPosition(glm::vec3(6,9,6));



		

		m_ActiveScene = VXEntities_MakeOrGetScene("ActiveScene");
		// Entity

		auto& square = m_ActiveScene->CreateEntity<ENTPropStaticSprite>("Green Square");
		
		m_ActiveScene->CreateEntity<ENTJsonStaticTilemap>("JsonStaticTilemap").Setup(&m_ActiveScene->CreateEntity<ENTEnvStaticTilemap>("Tilemap"), m_CameraController);
		
		
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
		RenderCommand::Init();
		
		t += ts;
		m_CameraController.m_ZoomLevel = 6.9f;
		
		m_ViewportFocused = true;

		VX_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, .1f });
		RenderCommand::Clear();



		Renderer2D::BeginScene(m_CameraController.GetCamera(), m_CameraController.m_ZoomLevel);
		

		Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		// Update scene
		m_ActiveScene->OnUpdate(ts);
		m_SquareEntity.pos = glm::vec3(0, sinf(t) * 100, 0);
		
		
		

		Renderer2D::EndScene();

		

		
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();
		m_ActiveScene->OnImGuiRender();
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
		

		
	}

}

