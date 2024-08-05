#include "vxpch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vertex {
	VertexArray* VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLVertexArray();
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}