#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Panels/SceneHierarchyPanel.h"
#include "Vertex/Core/Input.h"
#include "Vertex/Utils/Utils.h"
#include "Vertex/ImGui/ImGuizmoLink.h"
#include "Vertex/Math/Math.h"
#include <VXEntities/Scene/EditorCamera.h>

namespace Vertex {

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_CameraController.GetCamera().SetWindowSize(1280, 720);
		m_CameraController.GetCamera().SetPosition(glm::vec3(0,0,0));



		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = VXEntities_MakeOrGetScene("ActiveScene");

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(&m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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
		
		

		VX_PROFILE_FUNCTION();

		// Update
		if (m_ViewportFocused)
			m_EditorCamera.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Framebuffer->ClearAttachment(1, -1);

		Ref<Camera> camera;
		glm::mat4 transform;
		if (true)
		{
			//Renderer2D::BeginScene(*camera, transform);
			Renderer2D::BeginScene(m_EditorCamera.GetViewProjection());

			//Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			//Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			//Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			// Update scene
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			m_SquareEntity.pos = glm::vec3(0, sinf(t) * 100, 0);
			//VX_INFO("{0}", sinf(t));

			auto mx = ImGuiLink::GetMousePos().x;
			auto my = ImGuiLink::GetMousePos().y;
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = nullptr;
				for (Entity* ent : *m_ActiveScene)
				{
					int id = 0;
					for (char c : ent->GetID())
					{
						id += c;
					}

					if (id == pixelData)
					{
						VX_INFO("m_HoveredEntity is ", ent->name().c_str());
						m_HoveredEntity = ent;
						break;
					}
				}
			}


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
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(VX_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		
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
		auto viewportOffset = ImGuiLink::GetCursorPos(); // Includes tab bar

		m_ViewportFocused = ImGuiLink::IsWindowFocused();
		m_ViewportHovered = ImGuiLink::IsWindowHovered();

		glm::vec2 viewportPanelSize = ImGuiLink::GetContentRegionAvail();
		m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		if (m_ViewportSize != viewportPanelSize)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });

		auto windowSize = ImGuiLink::GetWindowSize();
		auto minBound = ImGuiLink::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		glm::vec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		if (ImGuiLink::BeginDragDropTarget())
		{
			if (const ImGuiLink::ImGuiPayload* payload = ImGuiLink::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGuiLink::EndDragDropTarget();
		}

		Entity* selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity != nullptr && m_GizmoType != -1)
		{
			ImGuizmoLink::SetOrthographic(false);
			ImGuizmoLink::SetDrawlist();

			ImGuizmoLink::SetRect(glm::vec4(0,0, ImGuiLink::GetWindowSize()));

			const auto& camera = cam->camera;
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			glm::mat4 transform = Vertex::Math::ComposeTransform(selectedEntity->pos, selectedEntity->size, selectedEntity->rotation);

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;

			if (m_GizmoType == ImGuizmoLink::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmoLink::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmoLink::OPERATION)m_GizmoType, ImGuizmoLink::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (true)
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - selectedEntity->rotation;
				selectedEntity->pos = translation;
				selectedEntity->rotation += deltaRotation;
				selectedEntity->size = scale;
			}
		}

		ImGuiLink::End();
		ImGuiLink::PopStyleVar();

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
		
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case KeyCode::N:
		{
			if (control)
				NewScene();

			break;
		}
		case Key::O:
		{
			if (control)
				OpenScene();

			break;
		}
		case Key::S:
		{
			if (control && shift)
				SaveSceneAs();

			break;
		}

		// Gizmos
		case Key::Q:
			m_GizmoType = -1;
			break;
		case Key::W:
			m_GizmoType = ImGuizmoLink::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_GizmoType = ImGuizmoLink::OPERATION::ROTATE;
			break;
		case Key::R:
			m_GizmoType = ImGuizmoLink::OPERATION::SCALE;
			break;
		}
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

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		NewScene();
		SceneSerializer serializer(&m_ActiveScene);
		serializer.Deserialize(path.string());
	}

}

