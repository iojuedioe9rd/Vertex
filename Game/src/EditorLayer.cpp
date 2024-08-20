#include "EditorLayer.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex/Core/Input.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "GridManager.h"
#include "Vertex/Audio/AudioManager.h"

#include <iostream>
#include <cstdlib>
#include <chrono>


namespace Vertex {

	struct EntityWave
	{
		int health;
		float speed;
		float sizeMod;
		int count;
		float rate;
	};

	struct Wave
	{
		std::vector<EntityWave> entityWaves;
		float rate;
	};

	Audio* music;

	std::vector<Entity*> Entitys;
	int waveIndex = 0;

	EditorLayer* EditorLayer::gameLayer;
	std::array< Wave, 5> waves;


	void SetupWaves()
	{
		Wave wave1 = Wave();
		wave1.entityWaves.emplace_back(EntityWave());
		wave1.rate = 1.0f;
		wave1.entityWaves[0].count = 5;
		wave1.entityWaves[0].health = 100;
		wave1.entityWaves[0].sizeMod = 0;
		wave1.entityWaves[0].speed = 1;
		wave1.entityWaves[0].rate = 1.0f;

		Wave wave2 = Wave();
		wave2.entityWaves.emplace_back(EntityWave());
		wave2.rate = 1.0f;
		wave2.entityWaves[0].count = 10;
		wave2.entityWaves[0].health = 200;
		wave2.entityWaves[0].sizeMod = 1;
		wave2.entityWaves[0].speed = 5;
		wave2.entityWaves[0].rate = 1.0f;

		Wave wave3 = Wave();
		wave3.entityWaves.emplace_back(EntityWave());
		wave3.rate = 1.0f;
		wave3.entityWaves[0].count = 20;
		wave3.entityWaves[0].health = 300;
		wave3.entityWaves[0].sizeMod = 2;
		wave3.entityWaves[0].speed = 10;
		wave3.entityWaves[0].rate = 1.0f;

		Wave wave4 = Wave();
		wave4.entityWaves.emplace_back(EntityWave());
		wave4.rate = 1.0f;
		wave4.entityWaves[0].count = 30;
		wave4.entityWaves[0].health = 300;
		wave4.entityWaves[0].sizeMod = 3;
		wave4.entityWaves[0].speed = 10;
		wave4.entityWaves[0].rate = 1.0f;

		Wave wave5 = Wave();
		wave5.entityWaves.emplace_back(EntityWave());
		wave5.entityWaves.emplace_back(EntityWave());
		wave5.rate = 1.0f;
		wave5.entityWaves[0].count = 100;
		wave5.entityWaves[0].health = 300;
		wave5.entityWaves[0].sizeMod = 3;
		wave5.entityWaves[0].speed = 10;
		wave5.entityWaves[0].rate = 1.0f;

		wave5.entityWaves[1].count = 1;
		wave5.entityWaves[1].health = 100;
		wave5.entityWaves[1].sizeMod = 10;
		wave5.entityWaves[1].speed = .5f;
		wave5.entityWaves[1].rate = 1.0f;

		waves[0] = wave1;
		waves[1] = wave2;
		waves[2] = wave3;
		waves[3] = wave4;
		waves[4] = wave5;
		
	}

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		Entitys = std::vector<Entity*>();

		waves = std::array<Wave, 5>();

		SetupWaves();

		tileMap = new TileMap();

		glm::i32vec2 size = glm::i32vec2(14, 8) * 10;
		gridManager = new GridManager(tileMap, 1, size.x, size.y, glm::i32vec2(-size.x * .5f, -size.y * .5f));


		gameLayer = this;

		music = Audio::Create("assets/music/tdhtth.wav", 1);
		music->Play();
	}

	EditorLayer::~EditorLayer()
	{
		
	}

	bool HayBad()
	{
		bool v = 0;

		for (Entity* e : Entitys)
		{

			if (v) { break; }
			if (e->getEntityType() == ENTITY_TYPE_EMEMY)
			{
				v = 1;
				break;
			}
		}

		return v;
	}

	void thread(EditorLayer* editorLayer)
	{
		int l = 1;
		while (true)
		{
			int waveIndex = 0;


			using namespace std::chrono_literals;
			typedef std::chrono::seconds s;


			for (; waveIndex < waves.size(); )
			{
				auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<float>(waves[waveIndex].rate));



				std::this_thread::sleep_for(duration);

				for (int i = 0; i < waves[waveIndex].entityWaves.size(); i++)
				{
					for (int j = 0; j < waves[waveIndex].entityWaves[i].count; j++)
					{
						duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<float>(waves[waveIndex].entityWaves[i].rate));

						std::this_thread::sleep_for(duration);

						Enemy* e = new Enemy(waves[waveIndex].entityWaves[i].health, waves[waveIndex].entityWaves[i].speed, 0.1f);
						e->setTex((editorLayer->m_UITextures[2]));

						Entitys.push_back(std::move(e));
					}


					bool s = 0;
					while (!s)
					{
						std::this_thread::sleep_for(1ms);
						if (!HayBad())
						{
							s = 1;

						}
					}

				}
				waveIndex++;


			}

			glm::i32vec2 size = glm::i32vec2(14, 8) * 10;
			editorLayer->tileMap->Clear();

			editorLayer->gridManager = new GridManager(editorLayer->tileMap, l, size.x, size.y, glm::i32vec2(-size.x * .5f, -size.y * .5f));
		}

		

		
	}

	

	std::thread* thread_obj;

	
	
	void EditorLayer::OnAttach()
	{
		VX_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Player.png");

		m_UITextures = std::array< Ref<Texture2D>, 3>();
		
		m_UITextures[0] = Texture2D::Create("assets/textures/Player.png");
		m_UITextures[1] = Texture2D::Create("assets/textures/T-UI.png");
		m_UITextures[2] = Texture2D::Create("assets/textures/Enemy.png");

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_CameraController.GetCamera().SetWindowSize(1280, 720);



		

		//Enemy* e = new Enemy(100, 5, 0.1f);

		
		//e->setTex(m_UITextures[2]);

		//Entitys.push_back(e);

		thread_obj = new std::thread(thread, this);
	}

	void EditorLayer::OnDetach()
	{
		VX_PROFILE_FUNCTION();
	}

	float t = 0;
	bool called;

	int cat_id;
	bool make_cat;

	
	int timer = 0;
	
	

	void EditorLayer::OnUpdate(Timestep ts)
	{
		

		VX_PROFILE_FUNCTION();

		if (t == 10.0f)
		{
			for (Entity* e : Entitys)
			{
				if (e == nullptr) { continue; }

				

				e->update(ts);
			}
		}
		else
		{
			t++;
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

			VX_INFO("{0}", money);

			VX_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
			Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

			

			tileMap->Draw();
			
			
			if (t == 10.0f)
			{
				for (Entity* e : Entitys)
				{
					if (e == nullptr) { continue; }
					e->draw(ts);
				}
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
			if (ImGuiLink::ImageButtonWithText((void*)m_UITextures[0]->GetRendererID(), "Basic cat.", glm::vec2(100, 100), 1, glm::vec2(0, 0), glm::vec2{ 0, 1 }, glm::vec2{ 1, 0 }))
			{
				
				{
					make_cat = true;
					cat_id = 0;
					money -= 50;
				}
				
			};
			//ImGuiLink::ImageButtonWithText((void*)m_UITextures[1]->GetRendererID(), "Turret Cat. 200 UNS", glm::vec2(100, 100), 1, glm::vec2(0, 0), glm::vec2{0, 1}, glm::vec2{1, 0});
			


			ImGuiLink::End();

			ImGuiLink::Begin("Info");

			

			if (isDie)
			{
				ImGuiLink::Text("Lol bro is die ):<");
			}

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

	void EditorLayer::AddMoney(int v)
	{
		money += v;
	}

	void EditorLayer::Kill()
	{
		isDie = true;
	}

}

