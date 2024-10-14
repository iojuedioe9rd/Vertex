#pragma once
#include "Vertex.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include <Vertex/Renderer/Framebuffer.h>
#include <Vertex/Renderer/Texture.h>
#include "VXEntities.h"
#include "Vertex/postprocessing.h"

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

		void LoadL(int l);

		void InitCrt();
		void drawCRT();

		static EditorLayer* GetMeEeeeeEEEeEeee()
		{
			retASMName e;
		}
		Ref<Texture2D> crt_front_screen_display_tex;
		void DockSpaceCallback();
	private:
		bool isPlayed = false;
		bool playBtn = false;
		static EditorLayer* e;
		
		Ref<Shader> m_crtShader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexArray> m_SquareVA;


		bool m_ViewportFocused = false, m_ViewportHovered = false;

		OrthographicCameraController m_CameraController;

		PostProcessing postProcessing;

		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Scene* m_L0;
		Scene* m_L1;
		ENTPropStaticSprite m_SquareEntity;


		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};

}