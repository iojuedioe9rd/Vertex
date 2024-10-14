#pragma once
#include "Vertex.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include <Vertex/Renderer/Framebuffer.h>
#include <Vertex/Renderer/Texture.h>
#include "VXEntities.h"

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
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Framebuffer* m_Framebuffer;

		ENTPointCamera2D* m_CameraEntity;
		ENTPointCamera2D* m_SecondCamera;

		bool m_PrimaryCamera = true;

		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Scene* m_ActiveScene;
		ENTPropStaticSprite m_SquareEntity;


		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}