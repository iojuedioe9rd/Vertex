#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entity.h"
#include "Player.h"
#include "GridManager.h"

GridManager* gridManager;

namespace Vertex {

	std::vector<Entity*> Entitys;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		Entitys = std::vector<Entity*>();

		tileMap = new TileMap();

		glm::i32vec2 size = glm::i32vec2(14, 8) * 10;
		gridManager = new GridManager(tileMap, size.x, size.y, glm::i32vec2(-size.x * .5f, -size.y * .5f));

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

		Player* player = new Player();

		

		Entitys.push_back(player);
	}

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		
		

		VX_PROFILE_FUNCTION();

		for (Entity* e : Entitys)
		{
			e->update(ts);
		}

		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		{
			
			VX_PROFILE_SCOPE("Renderer Prep");
			
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

			tileMap->Draw();

			for (Entity* e : Entitys)
			{
				e->draw(ts);
			}

			

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
		
			
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = 0;

		ImGuiLink::Docking(0, [this] { DockSpaceCallback(); });

		if (dockingEnabled == 0)
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

		ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGuiLink::End();

		ImGuiLink::Begin("Hi");
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() }, glm::vec2{0, 1}, glm::vec2{1, 0});
		ImGuiLink::End();

		
	}

}

