#include "vxpch.h"
#include "Framebuffer.h"

#include "Vertex/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Vertex {

	Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLFramebuffer(spec);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
