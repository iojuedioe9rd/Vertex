#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/Renderer/Texture.h"
#include "VertexArray.h"
#include "Shader.h"

VERTEX_BASE_STRUCT_BEGIN(MeshVertex);
glm::vec3 Position;
glm::vec3 Normal;
glm::vec4 Colour;
glm::vec2 TexCoords;

VERTEX_BASE_STRUCT_END();

VERTEX_BASE_STRUCT_BEGIN(MeshTexture2D);
Ref<Texture2D> tex;
std::string type;
VERTEX_BASE_STRUCT_END();

VERTEX_BASE_CLASS_BEGIN(Mesh);
public:
	Ref<VertexArray> vertexArray;
	std::vector<Ref<MeshTexture2D>> textures;

	Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<MeshTexture2D>> textures);
	void Draw(Ref<Shader> shader);
private:
	void SetupMesh(Ref<VertexBuffer> VB, Ref<IndexBuffer> IB);
VERTEX_BASE_CLASS_END();