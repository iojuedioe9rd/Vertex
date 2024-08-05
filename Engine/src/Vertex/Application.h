#pragma once
#include "Vertex/Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "ImGui/ImGuiWindowStack.h"
#include "ImGui/ImGuiWindows/ConsoleWindow.h"
#include "CommandBuffer/CommandBufferPool.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthographicCamera.h"

namespace Vertex
{

	class VERTEX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();

		Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		static Application& Get() { return *app; }

		CommandBuffer* GetCommandBuffer() { return m_CommandBufferPool->get(); }
		
		void ReownCommandBuffer(CommandBuffer* commandBuffer) { m_CommandBufferPool->reown(commandBuffer); }
		

		void OnEvent(Event* e);
	private:
		ImGuiLayer* m_ImGuiLayer;
		ImGuiWindows::ConsoleWindow* m_ConsoleWindow;
		static Application* app;
		Ref<Window> m_Window;
		bool m_Running = true;
		void Update();
		CommandBufferPool* m_CommandBufferPool;
		LayerStack* m_LayerStack = 0;
		ImGuiWindows::ImGuiWindowStack* m_ImGuiWindowStack = 0;

		

		

		OrthographicCamera m_Camera;
	};
	extern Vertex::Application* CreateApp();
	

}
