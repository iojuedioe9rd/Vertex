#include "vxpch.h"
#include "Application.h"
#include "Vertex/Events/ApplicationEvent.h"
#include "Vertex/Core/Logger.h"
#include "ErrorBox.h"
#include <glad/glad.h>
#include "Vertex/Renderer/Renderer.h"
#include "Vertex/Scripting/ScriptEngine.h"
#include "Vertex/Lua/LuaScripting.h"
#include <GLFW/glfw3.h>
#include <Vertex/Renderer/Renderer2D.h>
#include <Vertex/Core/Intro/IntroData.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
	// Your fuzzing logic here
	return 0;  // Return 0 if processed successfully
}

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
		: m_Camera(-1.6f * 2, 1.6f * 2, -0.9f * 2, 0.9f * 2), m_CommandLineArgs(args)
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
		{
			Timer t{};
			SetupIntroData();
			VX_CORE_INFO("SetupIntroData() {0}ms", t.ElapsedMillis());
		}

		Renderer::Init();
		ScriptEngine::Init();
		LuaScripting::Init();

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



	static float m_LastFrameTime = 0.0f;
	static int t = 0;
	static bool FirstRun = false;
	void Application::Update()
	{
		VX_PROFILE_FUNCTION();
		t++;
		float time = (float)Time::GetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		Time::m_Timestep = timestep;
		//m_Window->SetVSync(false);
		Time::FPS = GetFPS();

		ExecuteMainThreadQueue();

		//VX_CORE_INFO("{0}", timestep.GetMilliseconds());
		if (!m_IsInIntro)
		{
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
		}

		if (m_IsInIntro)
		{
			if (FirstRun) { m_IntroTimer = Timer(); }


			RenderCommand::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			RenderCommand::Clear();

			Renderer2D::BeginScene(m_Camera.GetViewProjectionMatrix());

			Renderer2D::DrawQuad(glm::vec2(0, 0), glm::vec2(1, 1), GetCatTexture());
			float offset = sinf(m_IntroTimer.ElapsedMillis() / 1000) / 10;

			Renderer2D::DrawQuad(glm::vec2(0.41, -0.2 + offset), glm::vec2(.3, .3), GetCircleTexture());
			Renderer2D::DrawQuad(glm::vec2(-0.46, -0.2 + offset), glm::vec2(.3, .3), GetCircleTexture());

			static bool camMoveUp = false;

			if (m_IntroTimer.ElapsedSeconds() >= 5.0f)
			{
				camMoveUp = true;
			}

			if (camMoveUp)
			{
				m_camPos += glm::vec3(0, 1, 0) * timestep.operator float();
				m_Camera.SetPosition(m_camPos);
			}

			Renderer2D::EndScene();
			if (m_IntroTimer.ElapsedSeconds() >= 14.999f)
			{
				m_IsInIntro = false;
			}
		}

		FirstRun = false;
		
		if(t > 10)
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
			return;
		}
		
		if (e->GetEventType() == EventType::MouseMoved)
		{
			delete e;
			return;
		}

		if (e->IsInCategory(EventCategoryApplication) && e->GetEventType() == EventType::WindowMoved)
		{
			Update();
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