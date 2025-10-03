#pragma once
#include "Vertex/Core/Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "Vertex/ImGui/ImGuiLayer.h"
#include "Vertex/ImGui/ImGuiWindowStack.h"
#include "Vertex/ImGui/ImGuiWindows/ConsoleWindow.h"
#include "Vertex/CommandBuffer/CommandBufferPool.h"
#include "Vertex/Renderer/Shader.h"
#include "Vertex/Renderer/Buffer.h"
#include "Vertex/Renderer/VertexArray.h"
#include "Vertex/Renderer/OrthographicCamera.h"

namespace Vertex
{
	struct VERTEX_API ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;
		const char* operator[](int index) const
		{
			VX_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct VERTEX_API ApplicationSettings
	{
		std::string Name = "Vertex App";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		ApplicationCommandLineArgs Args = ApplicationCommandLineArgs();
		bool VSync = true;
		bool EnablePlugins = true;
		SafeObject<int64_t> RandomSeed = SafeObject<int64_t>();
	};

	

	class VERTEX_API Application
	{
	public:
		Application(ApplicationSettings& settings = ApplicationSettings());
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();

		Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		static Application& Get() { return *app; }
		static glm::vec2 GetWindowSize() { return glm::vec2(app->m_Window->GetWidth(), app->m_Window->GetHeight()); }

		CommandBuffer* GetCommandBuffer() { return m_CommandBufferPool->get(); }
		
		void ReownCommandBuffer(CommandBuffer* commandBuffer) { m_CommandBufferPool->reown(commandBuffer); }
		
		void Close();

		void OnEvent(Event* e);

		void SubmitToMainThread(const std::function<void()>& function);

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

		void LoadDLL(const std::fs::path& path);
		Ref<DLLInstance> GetDLL(const std::fs::path& path);
	private:
		void SetupPlugins();
		void LoadDLLFromDirectory(const std::filesystem::directory_iterator directory_iterator, uint16_t i = 100);

		ApplicationSettings m_Settings;
		ApplicationCommandLineArgs m_CommandLineArgs;
		ImGuiLayer* m_ImGuiLayer;
		ImGuiWindows::ConsoleWindow* m_ConsoleWindow;
		static Application* app;
		Ref<Window> m_Window;
		bool m_Minimized = false;
		bool m_Running = true;
		void Update();
		CommandBufferPool* m_CommandBufferPool;
		LayerStack* m_LayerStack = 0;
		ImGuiWindows::ImGuiWindowStack* m_ImGuiWindowStack = 0;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;

		void ExecuteMainThreadQueue();

		std::vector<Ref<DLLInstance>> m_DLLs;

		OrthographicCamera m_Camera;
	};
	extern Vertex::Application* CreateApp(ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
	

}
