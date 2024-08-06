#include "vxpch.h"
#include "Renderer.h"
#include "Vertex/Core/Application.h"
#include "Vertex/CommandBuffer/CommandBufferPool.h"

namespace Vertex {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	CommandBuffer* commandBuffer;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		commandBuffer = Application::Get().GetCommandBuffer();
	}

	void Renderer::EndScene()
	{
		commandBuffer->executeCommands();
		commandBuffer->clear();
		Application::Get().ReownCommandBuffer(commandBuffer);
	}

	

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		//commandBuffer->addCommand(Submit_code, shader, vertexArray, transform);
		Submit_code(shader, vertexArray, transform);
	}
	void Renderer::Submit_code(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
		shader->Unbind();
	}
}