#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/Renderer/Texture.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Vertex {

	struct VERTEX_API MeshVertex 
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Colour;
		glm::vec2 TexCoords;

	};

	struct VERTEX_API MeshTexture2D 
	{
		Ref<Texture2D> tex;
		std::string type;
	};

	class VERTEX_API Mesh {

	public:

		static Ref<Mesh> Create(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<MeshTexture2D>> textures);

		//Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<MeshTexture2D>> textures);
		virtual void Draw(Ref<Shader> shader) = 0;
	private:
		//void SetupMesh(Ref<VertexBuffer> VB, Ref<IndexBuffer> IB);
	};

};