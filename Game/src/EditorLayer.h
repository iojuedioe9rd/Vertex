#pragma once
#include "Vertex.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include <Vertex/Renderer/Framebuffer.h>
#include <Vertex/Renderer/Texture.h>
#include "TileMap.h"
#include "Entity.h"
#include "GridManager.h"

namespace Vertex {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		std::vector<Entity*> GetEntitys();

		void RemoveE(Entity* e);

		void DockSpaceCallback();
		GridManager* GetGridManager() { return gridManager; }

		static EditorLayer Get() { return *gameLayer; }

		// £100 not $100
		int money = 100;

		void AddMoney(int v);
		
		static EditorLayer* gameLayer;
		GridManager* gridManager;
		OrthographicCameraController m_CameraController;
		TileMap* tileMap;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Framebuffer* m_Framebuffer;

		Ref<Texture2D> m_CheckerboardTexture;
		std::array<Ref<Texture2D>, 3> m_UITextures;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int baseHsrgfdg = 100;



		void damage(int v)
		{
			baseHsrgfdg -= v;
			if (baseHsrgfdg <= 0)
			{
				Kill();
			}
		}
	private:
		bool isDie = 0;
		void Kill();
		
	};

}