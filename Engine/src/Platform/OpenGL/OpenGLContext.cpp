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

#ifdef VX_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		VX_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Vertex requires at least OpenGL version 4.5!");
#endif

		
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}