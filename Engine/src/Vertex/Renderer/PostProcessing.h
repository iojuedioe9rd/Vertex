#pragma once
#if 0
#include <Vertex/Core/Base.h>
#include <Vertex/Renderer/Shader.h>
#include <Vertex/Renderer/Framebuffer.h>
#include <Vertex/Renderer/VertexArray.h>
#include <Vertex/Renderer/RenderCommand.h>

namespace Vertex
{


	class VERTEX_API PostProcssing
	{
	public:
		PostProcssing(std::filesystem::path copyShaderFilePath, FramebufferSpecification framebufferSpecification);
		~PostProcssing();

		void Add(Ref<Shader> shader);

		void Remove(Ref<Shader> shader);

		void Begin();
		void End();
		void UnbindFramebuffer();
		void Resize(uint32_t x, uint32_t y);
	private:

		struct VERTEX_API PostProcssingData
		{
			Ref<Framebuffer> m_Framebuffer;
			Ref<Shader> m_Shader;
		};

		std::vector<PostProcssingData> m_PostProcessings;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		FramebufferSpecification m_framebufferSpecification;
	};

	
}

#endif