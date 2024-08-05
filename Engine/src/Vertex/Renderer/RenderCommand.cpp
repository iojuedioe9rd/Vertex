#include "vxpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Vertex {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}