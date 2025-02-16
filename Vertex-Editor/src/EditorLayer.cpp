#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Panels/SceneHierarchyPanel.h"
#include "Vertex/Core/Input.h"
#include "Vertex/Utils/Utils.h"
#include "Vertex/ImGui/ImGuizmoLink.h"
#include "Vertex/Math/Math.h"
#include <Vertex/Scene/EditorCamera.h>
#include "resource.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <Vertex/Scripting/ScriptEngine.h>
#include <Vertex/Animation/AnimationLoader.h>
#include <Vertex/Renderer/Font.h>
#include <Vertex/AssetManager/AssetManager.h>
#include <Vertex/Scripting/ScriptGlue.h>
#include <Vertex/Renderer/UniformBuffer.h>



namespace Vertex {

	Ref< EditorAssetManager> m_AssetManager;
	

	// Vertices coordinates
	MeshVertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
		MeshVertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1), glm::vec2(0.0f, 0.0f)},
		MeshVertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1), glm::vec2(0.0f, 1.0f)},
		MeshVertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1), glm::vec2(1.0f, 1.0f)},
		MeshVertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f ,1), glm::vec2(1.0f, 0.0f)}
	};

	// Indices for vertices order
	uint32_t indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};


	extern const std::filesystem::path g_AssetPath;

	AssetHandle getAsset(std::filesystem::path path)
	{
		return m_AssetManager->ImportAsset(path);
	}

	AssetManagerBase* getAssetMan()
	{
		return m_AssetManager.get();
	}

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		m_AssetManager.reset(new EditorAssetManager("assets/AssetRegistry.vertexAssetRegistry"));
		AssetManager::SetAssetManager(m_AssetManager);
		m_AssetManager->DeserializeAssetRegistry();

		m_SceneHierarchyPanel = new SceneHierarchyPanel();
		m_ContentBrowserPanel = new ContentBrowserPanel("assets/cache");

		ScriptGlue::SetupGetingAssets(getAsset, getAssetMan);

		//test = Texture2DAnimated::Create("assets/textures/test.mp4");
		//test->SetFPS(2);
		//test->SetLooping(true);
		//test->SetSpeed(1.1f);
		VX_MAGIC_CALL();
	}

	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();
		
		
		//m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_CheckerboardTexture = GET_Texture2D_AssetManager("assets/textures/Checkerboard.png");
		// Resources/Icons/PlayButton.png
		m_IconPlay = GET_Texture2D_AssetManager("Resources/Icons/PlayButton.png");
		// Resources/Icons/StopButton.png
		m_IconStop = GET_Texture2D_AssetManager("Resources/Icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_CameraController.GetCamera().SetWindowSize(1280, 720);
		m_CameraController.GetCamera().SetPosition(glm::vec3(0,0,0));



		m_Framebuffer2 = Framebuffer::Create(fbSpec);
		//m_PostProcssing->Add(Shader::Create("assets/shaders/grayscale.glsl"));

		m_ActiveScene = new Scene("ActiveScene");
		m_EditorScene = new Scene("EditorScene");

		
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(&m_ActiveScene);
			//OpenScene(((EditorAssetManager*)(void*)AssetManager::GetAssetManager().get())->ImportAsset(sceneFilePath));
			m_EditorScene = m_ActiveScene;
		}
		else
		{
			m_ActiveScene = m_EditorScene;
			
            auto squareEntity = dynamic_cast<ENTPropStaticSprite*>(m_ActiveScene->CreateEntity("prop_static_sprite", "Green Square"));
            
			//m_ActiveScene->CreateEntity<ENTEnvStaticTilemap>("Tilemap").AddTile(glm::i32vec2(1, 5), nullptr, m_SquareColor);

			//m_CameraEntity = &m_ActiveScene->CreateEntity<ENTPointCamera2D>("Camera Entity");

			//m_CameraEntity->isPrimary = true;

			//m_SecondCamera = &m_ActiveScene->CreateEntity<ENTPointCamera2D>("Clip-Space Entity");

			//m_SecondCamera->isPrimary = false;




			squareEntity->colour = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
			squareEntity->SetIsVisible(true);

			
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		// Entity

		

		m_SceneHierarchyPanel->SetContext(m_ActiveScene);

		std::vector <MeshVertex> verts(vertices, vertices + sizeof(vertices) / sizeof(MeshVertex));
		std::vector <uint32_t> ind(indices, indices + sizeof(indices) / sizeof(uint32_t));
		
		std::vector<Ref<MeshTexture2D>> textures;

		m_testMesh.reset(new Mesh({
			// Front face
			{{-1.0f, -1.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f, -1.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-1.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

			// Back face
			{{-1.0f, -1.0f, -1.0f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f, -1.0f, -1.0f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f,  1.0f, -1.0f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-1.0f,  1.0f, -1.0f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

			// Left face
			{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

			// Right face
			{{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

			// Top face
			{{-1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{-1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{ 1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

			// Bottom face
			{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			}, {
				// Front face
				0, 1, 2, 2, 3, 0,
				// Back face
				4, 5, 6, 6, 7, 4,
				// Left face
				8, 9, 10, 10, 11, 8,
				// Right face
				12, 13, 14, 14, 15, 12,
				// Top face
				16, 17, 18, 18, 19, 16,
				// Bottom face
				20, 21, 22, 22, 23, 20
		}, textures));

		m_testShader = Shader::Create("assets/shaders/grayscale.glsl");
		m_testShader->Bind();
		m_testShader->Unbind();
		
		


		GImGui = (ImGuiContext*)ImGuiLink::GetContext();

		//m_Font.reset(new Font("assets/fonts/opensans/OpenSans-Regular.ttf"));
		//((EditorAssetManager*)(void*)g_AssetManagerBase.get())->ImportAsset("assets/scenes/Physics2D.vertex")
		//OpenScene(((EditorAssetManager*)(void*)g_AssetManagerBase.get())->ImportAsset("assets/scenes/Physics2D.vertex"));
	}

	float t = 0.0f;

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	void DrawFullScreenQuad(Ref<Shader> m_FlatColorShader, Framebuffer* m_Framebuffer2)
	{
		// Step 1: Define the vertex data for a full-screen quad
		float vertices[] = {
			// Positions         // Texture coordinates
			-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Top left
			-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom left
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom right
			 1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // Top right
		};

		unsigned int indices[] = {
			0, 1, 2, 0, 2, 3  // Two triangles to make a quad
		};

		// Step 2: Create the vertex buffer and index buffer
		Ref<VertexBuffer> vertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(indices, 6));

		// Step 3: Create a VertexArray to hold the buffers
		Ref<VertexArray> vertexArray = Ref<VertexArray>(VertexArray::Create());
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_UV" },
			});
		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);

		// Step 4: Upload the texture uniform and bind the shader

		

		//m_FlatColorShader->UploadUniformInt("u_Texture", 0); // Assuming 'u_Texture' is the uniform name in the shader

		m_FlatColorShader->Bind();

		// Bind the framebuffer texture to texture unit 0
		m_Framebuffer2->BindAsTex(0);

		// Step 5: Bind the vertex array and issue the draw call
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, indexBuffer->GetCount());
		vertexArray->Unbind();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (m_EditorScene == nullptr)
		{

		}

		
		
		
		t += ts;
		
		

		VX_PROFILE_FUNCTION();

		// Update
		//test->Update(ts.GetSeconds());

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		//m_PostProcssing->Begin();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		

		m_Framebuffer->ClearAttachment(1, -1);

		Ref<Camera> camera;
		glm::mat4 transform;
		if (true)
		{
			switch (m_SceneState)
			{
				case SceneState::Edit:
				{
					if (m_ViewportFocused)
						m_CameraController.OnUpdate(ts);
					m_EditorCamera.OnUpdate(ts);

					Renderer2D::BeginScene(m_EditorCamera.GetViewProjection());
					m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

					//Renderer2D::TextParams textParams;
					//textParams.Color = glm::vec4(1, 1, 1, 1);  // Set color, or any other parameters you need

					//Renderer2D::DrawString("Hello, World!", Font::GetDefault(), glm::mat4(1.0f), textParams);

					//Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1000.0f, 1000.0f }, test, 1.0f);
					RenderCommand::DisableDepthTesting();
					Renderer2D::EndScene();
					RenderCommand::EnableDepthTesting();

					break;
				}
				case SceneState::Play:
				{
					Ref<Camera> cam;
					glm::mat4 p_cam;
					if (m_ActiveScene->GetACameraInScene(&cam, true, &p_cam))
					{
						Renderer2D::BeginScene(cam->GetProjection(), p_cam);
						m_ActiveScene->OnUpdateRuntime(ts);
						//Renderer2D::TextParams textParams;
						//textParams.Color = glm::vec4(1, 1, 1, 1);  // Set color, or any other parameters you need

						

						//Renderer2D::DrawString("Hello, World!", Font::GetDefault(), glm::mat4(1), textParams);

						//Renderer2D::DrawCircle(glm::mat4(2.0f), textParams.Color);

						Renderer2D::EndScene();
					}
					
					break;
				}
			}
			//VX_INFO("{0}", sinf(t));
			/*
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
				if (pixelData == -1) return;
				m_HoveredEntity = nullptr;
				for (Entity* ent : *m_ActiveScene)
				{
					int id = 0;
					for (char c : ent->GetID())
					{
						id += c;
					}

					if (ent->GetIntID() == pixelData)
					{
						VX_INFO("m_HoveredEntity is ", ent->name().c_str());
						m_HoveredEntity = ent;
						break;
					}
				}
			}*/

			
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_EditorCamera.GetViewProjection());
			
			
			Renderer2D::EndScene();
		}

		

		//m_testShader->Bind();
		//m_testShader->UploadUniformFloat4("lightColor", glm::vec4(1));
		//m_testShader->UploadUniformMat4("camMatrix", m_EditorCamera.GetViewMatrix());
		//m_testShader->UploadUniformMat4("model", glm::mat4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));
		//m_testMesh->Draw(m_testShader);

		

		
		m_Framebuffer->Unbind();
		RenderCommand::DisableDepthTesting();

		//m_PostProcssing->UnbindFramebuffer();
		//m_Framebuffer->Bind();
		//m_testShader->Bind();

		//DrawFullScreenQuad(m_testShader, m_Framebuffer2);

		//m_PostProcssing->End();
		//m_Framebuffer->Unbind();

		RenderCommand::EnableDepthTesting();
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = 1;

		ImGuiLink::Docking(dockingEnabled, [this] { DockSpaceCallback(); });

		if (m_SceneState == SceneState::Play)
		{
			m_ActiveScene->OnImGuiDraw();
		}

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

			//uint32_t textureID = test->GetRendererID();
			//ImGuiLink::Image((void*)textureID, glm::vec2(100, 100), glm::vec2{0, 1}, glm::vec2{1, 0});

			

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
			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}
			ImGuiLink::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Image((ImTextureID)Font::GetDefault()->GetAtlasTexture()->GetRendererID(), {512,512}, {0, 1}, {1, 0});
		ImGui::Text("FPS: %d", static_cast<int>(std::round(Time::GetFPS())));

		if (true)
		{
			ImGuiLink::Separator();
			
			ImGuiLink::Text("%s", m_SquareEntity.name());

			m_SquareEntity.colour = m_SquareColor;
			ImGuiLink::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
			ImGuiLink::Separator();

		}
		//ImGuiLink::ColorEdit3("Camera Transform", glm::value_ptr(m_CameraEntity->pos));

		if (ImGuiLink::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity->isPrimary = m_PrimaryCamera;
			m_SecondCamera->isPrimary = !m_PrimaryCamera;

		}

		ImGui::End();

		ImGuiLink::PushStyleVar(ImGuiLink::ImGuiStyleVar_WindowPadding, glm::vec2{ 0, 0 });
		ImGuiLink::Begin("Viewport");
		auto viewportOffset = ImGuiLink::GetCursorPos(); // Includes tab bar

		m_ViewportFocused = ImGuiLink::IsWindowFocused();
		m_ViewportHovered = ImGuiLink::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);


		glm::vec2 viewportPanelSize = ImGuiLink::GetContentRegionAvail();
		m_ActiveScene->OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		if (m_ViewportSize != viewportPanelSize)
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_Framebuffer2->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			//m_PostProcssing->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGuiLink::Image((void*)textureID, glm::vec2{ viewportPanelSize.x, viewportPanelSize.y }, glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 });


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
				AssetHandle handle = *(AssetHandle*)payload->Data;
				OpenScene(handle);
			}
			ImGuiLink::EndDragDropTarget();
		}

		Entity* selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
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

		m_SceneHierarchyPanel->OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		UI_Toolbar();
		
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
		
		delete m_ActiveScene;
		m_ActiveScene = new Scene("ActiveScene");
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
		m_EditorScene = m_ActiveScene;
	}

	bool EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Vertex Scene (*.vertex)\0*.vertex\0");
		if (!filepath.empty())
		{
			
			OpenScene(((EditorAssetManager*)(void*)AssetManager::GetAssetManager().get())->ImportAsset(filepath));
			return true;
		}
		return false;
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

	void EditorLayer::OpenScene(AssetHandle handle)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		NewScene();

		Scene* readOnlyScene = (Scene*)(void*)(((EditorAssetManager*)(void*)AssetManager::GetAssetManager().get())->GetAsset(handle).get());
		Scene* newScene = Scene::Copy(readOnlyScene, std::string("ActiveScene"));

		//SceneSerializer serializer(&m_EditorScene);
		//if(serializer.Deserialize(path.string()));
		//{
		//	m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		//	m_SceneHierarchyPanel->SetContext(m_EditorScene);
//
	//		m_ActiveScene = m_EditorScene;
	//		m_EditorScenePath = path;
	//	}
		m_EditorScene = newScene;
		m_SceneHierarchyPanel->SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SerializeScene(Scene* scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(&scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene, std::string("Runtime"));
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;

		m_ActiveScene->OnRuntimeStop();
		delete m_ActiveScene;
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();

	}
	 
}

