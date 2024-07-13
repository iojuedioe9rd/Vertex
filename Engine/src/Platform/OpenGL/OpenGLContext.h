#pragma once
#include <GLFW/glfw3.h>
#include "Vertex/Renderer/GraphicsContext.h"

namespace Vertex
{
	class VERTEX_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}