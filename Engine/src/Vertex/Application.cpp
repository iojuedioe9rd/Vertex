#include "vxpch.h"
#include "Application.h"
#include "Vertex/Events/ApplicationEvent.h"
#include "Vertex/Logger.h"
#include <gl/GL.h>



namespace Vertex
{
	Application* Application::app = nullptr;
	Application::Application()
	{
		VX_PROFILE_FUNCTION();
		m_LayerStack = new LayerStack();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(VX_BIND_EVENT_FN(Vertex::Application::OnEvent));
		app = this;
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer)
	{
		
		VX_PROFILE_FUNCTION();
		m_LayerStack->PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		
		VX_PROFILE_FUNCTION();
		m_LayerStack->PushOverlay(layer);
		layer->OnAttach();
	}

	
	void Application::Update()
	{
		VX_PROFILE_FUNCTION();
		glClearColor(1, 0, 1, .01);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (Layer* layer : *m_LayerStack)
			layer->OnUpdate();
		
		
	}

	void Application::OnEvent(Event& e)
	{
		VX_PROFILE_FUNCTION();
		VX_TRACE("{0}", e.ToString());
		if (e.IsInCategory(EventCategoryApplication) && e.GetEventType() == EventType::WindowClose)
		{
			m_Running = 0;
		}

		if (e.IsInCategory(EventCategoryApplication) && e.GetEventType() == EventType::WindowResize)
		{
			
			
		}

		m_ImGuiLayer->OnEvent(e);


		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->OnEvent(e);
		}
	}



	Application::~Application()
	{
		delete m_LayerStack;
	}
	void Application::Run()
	{
		VX_PROFILE_FUNCTION();
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			VX_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			VX_TRACE(e.ToString());
		}
		


		while (m_Running)
		{
			Update();
			m_ImGuiLayer->Begin();
			for (Layer* layer : *m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
		

		
	}

	
}