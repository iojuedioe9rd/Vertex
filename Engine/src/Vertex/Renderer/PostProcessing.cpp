#include <vxpch.h>
#if 0
#include <Vertex/Renderer/PostProcessing.h>

namespace Vertex
{
	PostProcssing::PostProcssing(std::filesystem::path copyShaderFilePath, FramebufferSpecification framebufferSpecification)
	{
		// Step 1: Define the vertex data for a full-screen quad
		float vertices[] = {
			// Positions         // Texture coordinates
			-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Top left
			-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom left
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom right
			 1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // Top right
		};

		unsigned int indices[] = {
			0, 1, 2, 0, 2, 3  // Two triangles to make a quad
		};

		// Step 2: Create the vertex buffer and index buffer
		Ref<VertexBuffer> vertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(indices, 6));

		// Step 3: Create a VertexArray to hold the buffers
		m_VertexArray = Ref<VertexArray>(VertexArray::Create());
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_UV" },
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_framebufferSpecification = framebufferSpecification;
		m_Shader = Shader::Create(copyShaderFilePath.generic_string());
		m_Framebuffer = Ref<Framebuffer>(Framebuffer::Create(framebufferSpecification));
	}
	
	PostProcssing::~PostProcssing()
	{
	}
	void PostProcssing::Add(Ref<Shader> shader)
	{
		PostProcssingData data = {};

		data.m_Framebuffer = Ref<Framebuffer>(Framebuffer::Create(m_framebufferSpecification));
		data.m_Shader = shader;
		
		m_PostProcessings.push_back(data);
	}
	void PostProcssing::Remove(Ref<Shader> shader)
	{
	}
	void PostProcssing::Begin()
	{
		m_Framebuffer->Bind();
	}
	void PostProcssing::End()
	{
		// If there are no post-processing passes, just render the scene to the default framebuffer
		if (m_PostProcessings.empty())
		{
			m_Shader->Bind();
			m_Framebuffer->BindAsTex(0);
			m_Shader->UploadUniformInt("u_Texture", 0);
			m_VertexArray->Bind();
			RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
			return;
		}

		// Handle the first post-processing pass
		m_PostProcessings[0].m_Framebuffer->Bind();
		m_PostProcessings[0].m_Shader->Bind();
		m_Framebuffer->BindAsTex(0);
		m_PostProcessings[0].m_Shader->UploadUniformInt("u_Texture", 0);
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
		m_PostProcessings[0].m_Shader->Unbind();
		m_PostProcessings[0].m_Framebuffer->Unbind();

		// Handle the remaining post-processing passes (from 1 to size - 1)
		for (size_t i = 1; i < m_PostProcessings.size(); i++)
		{
			// Apply the current post-processing pass
			m_PostProcessings[i].m_Framebuffer->Bind();
			m_PostProcessings[i].m_Shader->Bind();
			m_PostProcessings[i - 1].m_Framebuffer->BindAsTex(0); // Use the previous pass's framebuffer as input
			m_PostProcessings[i].m_Shader->UploadUniformInt("u_Texture", 0);
			m_VertexArray->Bind();
			RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
			m_PostProcessings[i].m_Shader->Unbind();
			m_PostProcessings[i].m_Framebuffer->Unbind();

			// Display the result of the current post-processing pass
			m_Framebuffer->Bind();
			m_Shader->Bind();
			m_PostProcessings[i].m_Framebuffer->BindAsTex(0);
			m_Shader->UploadUniformInt("u_Texture", 0);
			m_VertexArray->Bind();
			RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
			m_Framebuffer->Unbind();
		}

		// Final post-processing pass (using the last pass's framebuffer)
		m_Shader->Bind();
		m_Framebuffer->BindAsTex(0);
		m_Shader->UploadUniformInt("u_Texture", 0);
		m_VertexArray->Bind();
		RenderCommand::DrawIndexed(m_VertexArray, m_VertexArray->GetIndexBuffer()->GetCount());
	}

	void PostProcssing::UnbindFramebuffer()
	{
		m_Framebuffer->Unbind();
	}
	void PostProcssing::Resize(uint32_t x, uint32_t y)
	{
		m_framebufferSpecification.Width = x;
		m_framebufferSpecification.Height = y;

		m_Framebuffer->Resize(x, y);

		for (size_t i = 0; i < m_PostProcessings.size(); i++)
		{
			m_PostProcessings[i].m_Framebuffer->Resize(x, y);
		}
	}
}

#endif