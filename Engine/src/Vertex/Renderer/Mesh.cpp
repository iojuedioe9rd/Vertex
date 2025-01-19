#include <vxpch.h>
#include "Mesh.h"
#include "RenderCommand.h"

//glm::vec3 Position;
//glm::vec3 Normal;
//glm::vec2 TexCoords;
//glm::vec4 Colour;

namespace Vertex
{
	Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<MeshTexture2D>> textures)
	{
		this->textures = textures;


		std::vector<float> verticesRawData = std::vector<float>();
        
		for (const MeshVertex& vertex : vertices)
		{
			verticesRawData.push_back(vertex.Position.x);
			verticesRawData.push_back(vertex.Position.y);
			verticesRawData.push_back(vertex.Position.z);

			verticesRawData.push_back(vertex.Normal.x);
			verticesRawData.push_back(vertex.Normal.y);
			verticesRawData.push_back(vertex.Normal.z);
			
			verticesRawData.push_back(vertex.TexCoords.x);
			verticesRawData.push_back(vertex.TexCoords.y);

			verticesRawData.push_back(vertex.Colour.r);
			verticesRawData.push_back(vertex.Colour.g);
			verticesRawData.push_back(vertex.Colour.b);
			verticesRawData.push_back(vertex.Colour.a);
			
		}

		Ref<VertexBuffer> VB = Ref<VertexBuffer>(VertexBuffer::Create(verticesRawData.data(), vertices.size() * sizeof(MeshVertex)));
		VB->SetLayout({

			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float4, "a_Colour" },
		});

		Ref<IndexBuffer> IB = Ref<IndexBuffer>(IndexBuffer::Create(indices.data(), indices.size()));
		
		SetupMesh(VB, IB);


	}

	void Mesh::SetupMesh(Ref<VertexBuffer> VB, Ref<IndexBuffer> IB)
	{
		vertexArray = Ref<VertexArray>(VertexArray::Create());
		vertexArray->AddVertexBuffer(VB);
		vertexArray->SetIndexBuffer(IB);
	}

	void Mesh::Draw(Ref<Shader> shader)
	{
		shader->Bind();
		vertexArray->Bind();

		uint32_t numDiff = 0;
		uint32_t numSpec = 0;

		for (uint32_t i = 0; i < textures.size(); i++)
		{
			std::string num;
			std::string type = textures[i]->type;
			if (type == "diffuse")
			{
				num = std::to_string(numDiff++);
			}
			else if (type == "specular")
			{
				num = std::to_string(numSpec++);
			}
			shader->UploadUniformInt("u_texture_" + (type + num), i);
			textures[i]->tex->Bind(i);
		}

		RenderCommand::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());

		vertexArray->Unbind();
		shader->Unbind();
	}
}


