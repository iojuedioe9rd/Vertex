#pragma once
#include <Vertex/Core/Base.h>
#include <Vertex/Renderer/Texture.h>
#include <unordered_map>


namespace Vertex {

	class VERTEX_API TextureManager2D
	{
	public:
		static Ref<Texture2D> GetTextureFromFilename(std::string filename);
		static Ref<Texture2D> GetOrMakeTextureFromFilename(std::string filename);
	private:
		static std::unordered_map<std::string, Ref<Texture2D>> m_TextureMap;
	};
}