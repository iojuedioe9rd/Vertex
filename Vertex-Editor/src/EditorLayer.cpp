#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entities/json_static_tilemap/json_static_tilemap.h"
#include <Vertex/Renderer/RenderCommand.h>
#include <Vertex/Audio/Audio.h>
#include <glad/glad.h>
#include <Platform/OpenGL/OpenGLFramebuffer.h>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;


std::atomic<bool> done(false);

#define MAX_LEVEL 2

int level = 0;

namespace Vertex {
	Audio* lol;
	struct LevelData
	{
		std::string name;
		uint8_t index;
		Scene* m_scene;
		std::thread* thread_object = nullptr;
		
		LevelData() = default;

		LevelData(uint8_t index, std::string name)
		{
			this->index = index;
			this->name = name;
		}
	};

	std::array<LevelData, MAX_LEVEL> maps = std::array<LevelData, MAX_LEVEL>();
	EditorLayer* EditorLayer::e;
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, 0, 0), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		
		e = this;
		for (uint8_t i = 0; i < MAX_LEVEL; i++)
		{
			std::string str = "map/map" + std::to_string(i + 1) + ".json";
			VX_INFO(str);
			maps[i] = LevelData(i, str);
			
		}

		
	}

	Scene* LoadMap(std::string filename, OrthographicCameraController& m_CameraController, std::thread** thread_object, Ref<Texture2D> crt_front_screen_display_tex, std::atomic<bool>& done) {
		Scene* scene = VXEntities_MakeOrGetScene(filename);

		// Entity creation
		auto& square = scene->CreateEntity<ENTPropStaticSprite>("Green Square");

		if (thread_object == nullptr)
		{
			scene->CreateEntity<ENTJsonStaticTilemap>("JsonStaticTilemap").ReDoSetup(filename, &scene->CreateEntity<ENTEnvStaticTilemap>("Tilemap"), m_CameraController, done);
			return scene;
		}

		// Properly assign the thread object from ReDoSetup
		*thread_object = scene->CreateEntity<ENTJsonStaticTilemap>("JsonStaticTilemap").ReDoSetup(filename, &scene->CreateEntity<ENTEnvStaticTilemap>("Tilemap"), m_CameraController, done);

		
		// Check if thread was assigned correctly
		if (*thread_object != nullptr) {
			std::cout << "Thread object assigned successfully!" << std::endl;
		}
		else {
			std::cerr << "Thread object is NULL!" << std::endl;
		}

		return scene;
	}



	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		
		//InitCrt();

		m_CameraController.GetCamera().SetWindowSize(1280, 720);
		m_CameraController.GetCamera().SetPosition(glm::vec3(6,9,6));

		lol = Audio::Create("assets/music/Overflow Error In 0x0053674.wav", true);
		
		lol->SetVolume(1.5f);
		
		
		crt_front_screen_display_tex = Texture2D::Create("PNG", IDB_PNG4);

		


		// Entity

		
		for (int i = 0; i < MAX_LEVEL; i++)
		{
			LevelData& l = maps[i];

			l.m_scene = LoadMap(l.name, m_CameraController, &l.thread_object, crt_front_screen_display_tex, done);
			if (i == 0)
			{
				l.thread_object->join();
			}
			maps[i] = l;
		}

		lol->Play();
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
		
		RenderCommand::SetClearColor({ 0.01f, 0.01f, 0.01f, .1f });
		RenderCommand::Clear();


		
		Renderer2D::BeginScene(m_CameraController.GetCamera(), m_CameraController.m_ZoomLevel, t);
		
		
		Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		
		// Update scene
		if (done && playBtn)
		{
			maps[level].m_scene->OnUpdate(ts);
		}
		
		m_SquareEntity.pos = glm::vec3(0, sinf(t) * 100, 0);
		
		Renderer2D::DrawQuad({ 0,0 }, { 100000, 100000 }, { 1,1,1,1 });
		Renderer2D::DrawQuad({ m_CameraController.m_CameraPosition.x, m_CameraController.m_CameraPosition.y }, {1, 1}, crt_front_screen_display_tex, 1, {1, 1, 1, 1}, true);
		
		

		Renderer2D::EndScene();

		
		
		//crt_front_screen_display_tex->Bind();
		

		//m_crtShader->Bind();
		//m_crtShader->UploadUniformMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
		//m_crtShader->UploadUniformMat4("u_Transform", glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//m_SquareVA->Bind();
		//RenderCommand::DrawIndexed(m_SquareVA);
		//m_SquareVA->Unbind();
		//m_crtShader->Unbind();
		

		

		

		
	}

	void EditorLayer::OnImGuiRender()
	{
		VX_PROFILE_FUNCTION();
		if (isPlayed && playBtn)
		{
			maps[level].m_scene->OnImGuiRender();
		}
		else
		{

			ImGuiLink::Begin("Game Info");
			if (playBtn || ImGuiLink::Button("Play"))
			{
				playBtn = true;
				

				
			}
			if (playBtn)
			{
				ImGuiLink::Text("Loading");
			}
			ImGuiLink::End();
		}
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
			ImGuiLink::Text("Texture: %d", stats.TextureCount);
			ImGuiLink::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGuiLink::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGuiLink::Text("Quads Percent: %d", (((float)stats.QuadCount / RENDERER2D_MAX_QUADS)));
			

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

	void EditorLayer::LoadL(int l)
	{

	}

	void EditorLayer::InitCrt()
	{
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};


		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);







		m_crtShader = Shader::Create("assets/shaders/crt front screen display shader.glsl");




		(m_crtShader)->Bind();
		(m_crtShader)->UploadUniformInt("u_Texture", 0);
	}

	void EditorLayer::drawCRT()
	{
		
	}


	void EditorLayer::DockSpaceCallback()
	{
		

		
	}

}

