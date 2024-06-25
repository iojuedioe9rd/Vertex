#pragma once
#include "Vertex/Core.h"
#include "Window.h"
#include "LayerStack.h"


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

		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		void Update();
		LayerStack* m_LayerStack = 0;
	};
	extern Vertex::Application* CreateApp();
	

}
