#include "vxpch.h"
#include "WindowsWindow.h"
#include <Vertex/Events/KeyEvent.h>
#include <Vertex/Events/ApplicationEvent.h>
#include "Platform/OpenGL/OpenGLContext.h"
#include <Vertex/Events/MouseEvent.h>

#include <glad/glad.h>
#include <stb_image.h>
// IDB_BITMAP1 104
#include <windows.h>
#include <glfw/glfw3.h>

unsigned char* LoadPNGFromResource(int resourceID, int* width, int* height, int* channels) {
	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceID), L"PNG");
	if (!hResource) {
		std::cerr << "Failed to find PNG resource." << std::endl;
		return nullptr;
	}

	HGLOBAL hMemory = LoadResource(NULL, hResource);
	if (!hMemory) {
		std::cerr << "Failed to load resource." << std::endl;
		return nullptr;
	}

	void* pData = LockResource(hMemory);
	DWORD size = SizeofResource(NULL, hResource);
	if (!pData || size == 0) {
		std::cerr << "Failed to lock resource." << std::endl;
		return nullptr;
	}

	unsigned char* imageData = stbi_load_from_memory((unsigned char*)pData, size, width, height, channels, STBI_rgb_alpha);
	if (!imageData) {
		std::cerr << "Failed to load PNG from resource." << std::endl;
	}

	return imageData;
}


namespace Vertex {

	static uint8_t s_GLFWWindowCount = 0;

	Window* Window::Create(const WindowProps& props)
	{
		VX_PROFILE_FUNCTION();
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		VX_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		VX_PROFILE_FUNCTION();
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		VX_PROFILE_FUNCTION();
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VX_CORE_INFO("Creating window: {0} ({1}, {2}, {3})", props.Title, props.Width, props.Height, (float)props.Width / (float)props.Height);

		if (s_GLFWWindowCount == 0)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			VX_CORE_ASSERT(success, "Could not intialize GLFW!");

			
		}
		 

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		GLFWimage images[1];  // Array of GLFWimage (you can have more icons if needed)
		int width, height, channels;

		unsigned char* pixels = LoadPNGFromResource(104, &width, &height, &channels);  // Load the bitmap resource

		if (pixels) {
			images[0].width = width;
			images[0].height = height;
			images[0].pixels = pixels;

			// Set the window icon
			glfwSetWindowIcon(m_Window, 1, images);

			// Free the image data after setting the icon
			stbi_image_free(pixels);  // Free image memory after use
		}
		else {
			std::cerr << "Failed to load the window icon." << std::endl;
		}
		s_GLFWWindowCount++;
		glfwMakeContextCurrent(m_Window);
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent* event = new WindowResizeEvent(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent* event = new WindowCloseEvent();
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent* event = new KeyPressedEvent(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent* event = new KeyReleasedEvent(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent* event = new KeyPressedEvent(key, true);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent* event = new KeyTypedEvent(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{

				case GLFW_PRESS:
				{
					MouseButtonPressedEvent* event = new MouseButtonPressedEvent(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent* event =  new MouseButtonReleasedEvent(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		VX_PROFILE_FUNCTION();
		s_GLFWWindowCount--;
		if (--s_GLFWWindowCount <= 0)
		{
			s_GLFWWindowCount = 0;
			VX_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
		
	}

	void WindowsWindow::OnUpdate()
	{
		VX_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		VX_PROFILE_FUNCTION();
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		VX_PROFILE_FUNCTION();
		return m_Data.VSync;
	}

}