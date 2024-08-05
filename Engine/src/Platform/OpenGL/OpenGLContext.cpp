#include "vxpch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>

namespace Vertex
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		VX_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VX_CORE_INFO("GLAD STATUS: {0}", status);
		VX_CORE_ASSERT(status, "Failed to initialize Glad!");

		VX_CORE_INFO("OpenGL Info:");
		VX_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		VX_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		VX_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}