#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Panels/SceneHierarchyPanel.h"
#include "Vertex/Core/Input.h"
#include "Vertex/Utils/Utils.h"

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
		
		m_ActiveScene->CreateEntity<ENTEnvStaticTilemap>("Tilemap").AddTile(glm::i32vec2(1, 5), nullptr, m_SquareColor);
		
		m_CameraEntity = &m_ActiveScene->CreateEntity<ENTPointCamera2D>("Camera Entity");
		
		m_CameraEntity->isPrimary = true;

		m_SecondCamera = &m_ActiveScene->CreateEntity<ENTPointCamera2D>("Clip-Space Entity");
		
		m_SecondCamera->isPrimary = false;

		
		square.colour = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
		square.SetIsVisible(true);

		m_SquareEntity = square;
		m_SquareEntity.colour = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		
		
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
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Ref<Camera> camera;
		glm::mat4 transform;
		if (m_ActiveScene->GetACameraInScene(&camera, true, &transform, true))
		{
			Renderer2D::BeginScene(*camera, transform);


			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			// Update scene
			m_ActiveScene->OnUpdate(ts);
			m_SquareEntity.pos = glm::vec3(0, sinf(t) * 100, 0);
			//VX_INFO("{0}", sinf(t));



			Renderer2D::EndScene();
		}
		

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = 1;

		ImGuiLink::Docking(dockingEnabled, [this] { DockSpaceCallback(); });

		if (0)
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
		if (ImGuiLink::BeginMenuBar())
		{
			if (ImGuiLink::BeginMenu("File"))
			{
				if (ImGuiLink::MenuItem("Serialize"))
				{
					SaveSceneAs();
				}

				if (ImGuiLink::MenuItem("Deserialize"))
				{
					OpenScene();
				}
				ImGuiLink::EndMenu();
			}
			ImGuiLink::EndMenuBar();
		}

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
		ImGuiLink::ColorEdit3("Camera Transform", glm::value_ptr(m_CameraEntity->pos));

		if (ImGuiLink::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity->isPrimary = m_PrimaryCamera;
			m_SecondCamera->isPrimary = !m_PrimaryCamera;

		}

		ImGuiLink::End();

		ImGuiLink::PushStyleVar(ImGuiLink::ImGuiStyleVar_WindowPadding, glm::vec2{ 0, 0 });
		ImGuiLink::Begin("Viewport");

		m_ViewportFocused = ImGuiLink::IsWindowFocused();
		m_ViewportHovered = ImGuiLink::IsWindowHovered();

		glm::vec2 viewportPanelSize = ImGuiLink::GetContentRegionAvail();
		m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		if (m_ViewportSize != viewportPanelSize)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			//m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });

		ImGuiLink::End();
		ImGuiLink::PopStyleVar();

		m_SceneHierarchyPanel.OnImGuiRender();

		
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	void EditorLayer::NewScene()
	{
		
		VXEntities_RemoveScene(m_ActiveScene);
		m_ActiveScene = VXEntities_MakeOrGetScene("ActiveScene");
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Vertex Scene (*.vertex)\0*.vertex\0");
		if (!filepath.empty())
		{
			NewScene();
			SceneSerializer serializer(&m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Vertex Scene (*.vertex)\0*.vertex\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(&m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}

