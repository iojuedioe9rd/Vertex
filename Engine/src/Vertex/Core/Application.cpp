#include "vxpch.h"
#include "Application.h"
#include "Vertex/Events/ApplicationEvent.h"
#include "Vertex/Core/Logger.h"
#include "ErrorBox.h"
#include <glad/glad.h>
#include "Vertex/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case Vertex::ShaderDataType::Float:    return GL_FLOAT;
	case Vertex::ShaderDataType::Float2:   return GL_FLOAT;
	case Vertex::ShaderDataType::Float3:   return GL_FLOAT;
	case Vertex::ShaderDataType::Float4:   return GL_FLOAT;
	case Vertex::ShaderDataType::Mat3:     return GL_FLOAT;
	case Vertex::ShaderDataType::Mat4:     return GL_FLOAT;
	case Vertex::ShaderDataType::Int:      return GL_INT;
	case Vertex::ShaderDataType::Int2:     return GL_INT;
	case Vertex::ShaderDataType::Int3:     return GL_INT;
	case Vertex::ShaderDataType::Int4:     return GL_INT;
	case Vertex::ShaderDataType::Bool:     return GL_BOOL;
	}

	VX_CORE_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}


using namespace Vertex::ImGuiWindows;
namespace Vertex
{

	void log(const std::string& msg) {
		VX_CORE_INFO(msg);
	}
	Application* Application::app = nullptr;

	
	Application::Application(const std::string& name, uint32_t width, uint32_t height, ApplicationCommandLineArgs args)
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CommandLineArgs(args)
	{
		
		
		
		VX_PROFILE_FUNCTION();
		m_LayerStack = new LayerStack();
		m_CommandBufferPool = new CommandBufferPool();
		m_ImGuiWindowStack = new ImGuiWindowStack();
		m_ConsoleWindow = &ConsoleWindow::Get();
		m_ImGuiWindowStack->PushImGuiWindow(m_ConsoleWindow);
		m_Window = Ref<Window>(Window::Create(WindowProps(name, width, height)));
		m_Window->SetEventCallback(VX_BIND_EVENT_FN(Vertex::Application::OnEvent));
		app = this;

		Renderer::Init();

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

	float prevTime = 0.0;
	float crntTime = 0.0;
	float timeDiff;
	uint32_t counter = 0;

	float GetFPS()
	{
		crntTime = Time::GetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			float FPS = ((1.0 / timeDiff) * counter);
			prevTime = crntTime;
			counter = 0;
			return FPS;
		}

		return Time::GetFPS();
	}


	float m_LastFrameTime = 0.0f;
	void Application::Update()
	{
		VX_PROFILE_FUNCTION();
		
		float time = (float)Time::GetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		Time::m_Timestep = timestep;
		//m_Window->SetVSync(false);
		Time::FPS = GetFPS();

		ExecuteMainThreadQueue();

		//VX_CORE_INFO("{0}", timestep.GetMilliseconds());
		

		if (!m_Minimized)
		{
			for (Layer* layer : *m_LayerStack)
				layer->OnUpdate(timestep);
		}

		
		m_ImGuiLayer->Begin();
		for (Layer* layer : *m_LayerStack)
			layer->OnImGuiRender();
		for (BaseImGuiWindow* imGuiWindow : *m_ImGuiWindowStack)
			imGuiWindow->OnUpdate();
		m_ImGuiLayer->End();
		
		m_Window->OnUpdate();
	}

	void Application::Close()
	{
		m_Running = 0;
	}

	void Application::OnEvent(Event* e)
	{
		VX_PROFILE_FUNCTION();

		if (e->IsInCategory(EventCategoryApplication) && e->GetEventType() == EventType::WindowClose)
		{
			m_Running = 0;
		}

		if (e->IsInCategory(EventCategoryApplication) && e->GetEventType() == EventType::WindowResize)
		{
			VX_CORE_INFO("Window Resized");
			Update();
			WindowResizeEvent resizeE = (*(WindowResizeEvent*)(void*)(e));

			if (resizeE.GetWidth() == 0 || resizeE.GetHeight() == 0)
			{
				m_Minimized = true;
			}
			else
			{
				m_Minimized = false;
				Renderer::OnWindowResize(resizeE.GetWidth(), resizeE.GetHeight());
			}
			
		}

		


		for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
		{
			if (e->handled)
				break;
			(*it)->OnEvent(*e);
		}

		delete e;
		
	}

	void Application::SubmitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);
		m_MainThreadQueue.emplace_back(function);
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}



	Application::~Application()
	{
		delete m_LayerStack;
	}
	void Application::Run()
	{
		VX_PROFILE_FUNCTION();
		
		


		while (m_Running)
		{
			Update();
			
			
		}
		

		
	}

	
}