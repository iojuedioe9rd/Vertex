#include "vxpch.h"
#include "Application.h"
#include "Vertex/Events/ApplicationEvent.h"
#include "Vertex/Logger.h"
#include <gl/GL.h>
#include "CustomFileFormat/FakeFS.h"
#include "CustomFileFormat/RIFF.h"
#include "ErrorBox.h"

using namespace Vertex::ImGuiWindows;
namespace Vertex
{

	void log(const std::string& msg) {
		VX_CORE_INFO(msg);
	}
	Application* Application::app = nullptr;
	Application::Application()
	{
		/*
		{
			FakeFS fakefs;
			fakefs.addFile("path/to/texture.png", File("DATA", "This is the content of texture.png."));
			fakefs.addFile("path/to/model.obj", File("DATA", "This is the content of model.obj."));
			fakefs.addFile("path/to/script.lua", File("DATA", "This is the content of script.lua."));
			
			FakeFS fakefs2;
			fakefs2.addFile("path/to/texture1.png", File("DATA", "This is the content of texture1.png."));
			fakefs2.addFile("path/to/model1.obj", File("DATA", "This is the content of model1.obj."));
			fakefs2.addFile("path/to/script1.lua", File("DATA", "This is the content of script1.lua.", "This is data"));

			fakefs.addFS(fakefs2);

			// Define the encryption key
			std::string encryptionKey = "my_secret_key";

			// Write the custom RIFF file with encryption
			writeRIFFFile("vertex_assets.vertex", fakefs, encryptionKey);

			// Create a new fake file system to read into
			FakeFS readFakefs;

			// Read the custom RIFF file with decryption
			try {
				readRIFFFile("vertex_assets.riff", readFakefs, encryptionKey);
			}
			catch (const std::runtime_error& e) {
				std::cerr << e.what() << std::endl;
				
			}

			// Read and print the content of the files
			try {
				std::string content1 = readFile(readFakefs, "path/to/texture.png");
				std::cout << "Content of texture.png: " << content1 << std::endl;

				std::string content2 = readFile(readFakefs, "path/to/model.obj");
				std::cout << "Content of model.obj: " << content2 << std::endl;

				std::string content3 = readFile(readFakefs, "path/to/script.lua");
				std::cout << "Content of script.lua: " << content3 << std::endl;

				std::string content4 = readFile(readFakefs, "path/to/texture1.png");
				std::cout << "Content of texture1.png: " << content1 << std::endl;

				std::string content5 = readFile(readFakefs, "path/to/model1.obj");
				std::cout << "Content of model1.obj: " << content2 << std::endl;

				std::string content6 = readFile(readFakefs, "path/to/script1.lua");
				std::cout << "Content of script1.lua: " << content3 << std::endl;

				
			}
			catch (const std::runtime_error& e) {
				std::cerr << e.what() << std::endl;
			}

		}
		*/
		VX_PROFILE_FUNCTION();
		m_LayerStack = new LayerStack();
		m_ImGuiWindowStack = new ImGuiWindowStack();
		m_ConsoleWindow = &ConsoleWindow::Get();
		m_ImGuiWindowStack->PushImGuiWindow(m_ConsoleWindow);
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

		m_ImGuiLayer->Begin();
		for (Layer* layer : *m_LayerStack)
			layer->OnImGuiRender();
		for (BaseImGuiWindow* imGuiWindow : *m_ImGuiWindowStack)
			imGuiWindow->OnUpdate();
		m_ImGuiLayer->End();
		
		m_Window->OnUpdate();
	}

	void Application::OnEvent(Event& e)
	{
		VX_PROFILE_FUNCTION();
		
		if (e.IsInCategory(EventCategoryApplication) && e.GetEventType() == EventType::WindowClose)
		{
			m_Running = 0;
		}

		if (e.IsInCategory(EventCategoryApplication) && e.GetEventType() == EventType::WindowResize)
		{
			
			Update();
		}

		


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
		
		


		while (m_Running)
		{
			Update();
			
			
		}
		

		
	}

	
}