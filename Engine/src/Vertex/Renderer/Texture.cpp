#include "vxpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Vertex 
{
	

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, Buffer* data)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(specification, data);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::CreateWin(int resID, const std::string& format)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(resID, format);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	/*
	Ref<Texture2DAnimated> Texture2DAnimated::Create(uint32_t width, uint32_t height, uint16_t frames)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2DAnimated>(width, height, frames);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2DAnimated> Texture2DAnimated::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    VX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2DAnimated>(path);
		}

		VX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	*/

}

