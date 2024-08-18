#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "GridManager.h"



namespace Vertex {

	struct EntityWave
	{
		int health;
		float speed;
		float sizeMod;
		int count;
	};

	struct Wave
	{
		EntityWave entityWave;
		float rate;
	};

	std::vector<Entity*> Entitys;
	int waveIndex = 0;

	EditorLayer* EditorLayer::gameLayer;
	std::array< Wave, 12> waves;


	void SetupWaves()
	{
		Wave wave1 = Wave();
		wave1.entityWave = EntityWave();
		wave1.rate = .5f;
		wave1.entityWave.count = 5;
		wave1.entityWave.health = 100;
		wave1.entityWave.sizeMod = 0;
		wave1.entityWave.speed = 1;

		waves[0] = wave1;
		
	}

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		Entitys = std::vector<Entity*>();

		SetupWaves();

		tileMap = new TileMap();

		glm::i32vec2 size = glm::i32vec2(14, 8) * 10;
		gridManager = new GridManager(tileMap, size.x, size.y, glm::i32vec2(-size.x * .5f, -size.y * .5f));


		gameLayer = this;
	}

	EditorLayer::~EditorLayer()
	{
		
	}

	Enemy* e;
	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Player.png");

		m_UITextures = std::array< Ref<Texture2D>, 2>();
		
		m_UITextures[0] = Texture2D::Create("assets/textures/Player.png");
		m_UITextures[1] = Texture2D::Create("assets/textures/T-UI.png");


		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_CameraController.GetCamera().SetWindowSize(1280, 720);



		

		e = new Enemy(100, 5, 0.1f);

		
		

		Entitys.push_back(e);

		
	}

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	float t = 0;
	bool called;

	int cat_id;
	bool make_cat;

	void EditorLayer::OnUpdate(Timestep ts)
	{
		t += ts;
		if (t >= 5)
		{
			e = new Enemy(100, 5, 0);
			Entitys.push_back(e);
			t = 0;
		}

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

			
			

			if (make_cat)
			{
				switch (cat_id)
				{
				case 0:
				{
					Renderer2D::DrawQuad(m_CameraController.GetCamera().PixelPosToWorldPos(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 0)), glm::vec2(1, 1), m_UITextures[0]);

					if (Input::IsMouseButtonPressed(VX_MOUSE_BUTTON_LEFT))
					{
						make_cat = 0;
						Player* p = new Player();
						p->setPos(m_CameraController.GetCamera().PixelPosToWorldPos(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 0)).x, m_CameraController.GetCamera().PixelPosToWorldPos(glm::vec3(Input::GetMouseX(), Input::GetMouseY(), 0)).y);
						Entitys.push_back(p);
					}
					break;
				}
				default:
					break;
				}
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
			ImGuiLink::End();

			ImGuiLink::Begin("Shop");

			uint32_t textureID = m_CheckerboardTexture->GetRendererID();
			if (ImGuiLink::ImageButtonWithText((void*)m_UITextures[0]->GetRendererID(), "Basic cat. 50 UNS", glm::vec2(100, 100), 1, glm::vec2(0, 0), glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 }))
			{
				if (money >= 50)
				{
					make_cat = true;
					cat_id = 0;
					money -= 50;
				}
				
			};
			ImGuiLink::ImageButtonWithText((void*)m_UITextures[1]->GetRendererID(), "Turret Cat. 200 UNS", glm::vec2(100, 100), 1, glm::vec2(0, 0), glm::vec2{0, 1}, glm::vec2{1, 0});
			
			ImGuiLink::End();

			
		}
		
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		
	}

	std::vector<Entity*> EditorLayer::GetEntitys()
	{
		return Entitys;
	}

	void EditorLayer::RemoveE(Entity* e)
	{
		int i = 0;
		for (Entity* tryE : Entitys)
		{
			
			if (tryE == e)
			{
				delete tryE;
				Entitys.erase(Entitys.begin() + i);
			}
			i++;
		}
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

