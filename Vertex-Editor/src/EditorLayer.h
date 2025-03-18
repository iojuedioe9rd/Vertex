#pragma once
#include "Vertex.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include <Vertex/Renderer/Framebuffer.h>
#include <Vertex/Renderer/Texture.h>
#include "Vertex/Scene/Entities/Entities.h"
#include "Panels/SceneHierarchyPanel.h"
#include <Vertex/Events/KeyEvent.h>
#include <Vertex/Events/Event.h>
#include <Vertex/Scene/EditorCamera.h>
#include "Panels/ContentBrowserPanel.h"
#include "Vertex/Renderer/Mesh.h"
#include "Vertex/Renderer/Font.h"
#include <filesystem>
#include <Vertex/AssetManager/EditorAssetManager.h>
#include <Vertex/Audio/Audio.h>
#include <Vertex/Core/Timer.h>


namespace Vertex {



	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void DockSpaceCallback();
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
		bool OpenScene();
		void SaveSceneAs();
		void OpenScene(AssetHandle handle);
		void SaveScene();

		void SerializeScene(Scene* scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();
	private:
		Timer m_Timer;
		

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		Ref<Mesh> m_testMesh;
		Ref<Font> m_Font;
		Ref<Shader> m_testShader;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Framebuffer* m_Framebuffer;

		ENTPointCamera2D* m_CameraEntity;
		ENTPointCamera2D* m_SecondCamera;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		Entity* m_HoveredEntity = nullptr;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;

		Ref<Texture2DAnimated> test;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		Scene* m_ActiveScene;
		Scene* m_EditorScene = nullptr;
		std::filesystem::path m_EditorScenePath;
		ENTPropStaticSprite m_SquareEntity;

		ENTPointCamera2D* cam;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel* m_SceneHierarchyPanel;
		ContentBrowserPanel* m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = BIT(0),
			Play = BIT(1)
		};

		SceneState m_SceneState = SceneState::Edit;
	};

}