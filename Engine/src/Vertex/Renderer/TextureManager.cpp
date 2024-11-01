#include "vxpch.h"
#include "TextureManager.h"

namespace Vertex
{
	std::unordered_map<std::string, Ref<Texture2D>> TextureManager2D::m_TextureMap = std::unordered_map<std::string, Ref<Texture2D>>();


	Ref<Texture2D> TextureManager2D::GetTextureFromFilename(std::string filename)
	{
		if (m_TextureMap.find(filename) != m_TextureMap.end())
		{
			return m_TextureMap[filename];
		}
		return Ref<Texture2D>();
	}

	Ref<Texture2D> TextureManager2D::GetOrMakeTextureFromFilename(std::string filename)
	{
		if (m_TextureMap.find(filename) != m_TextureMap.end())
		{
			return m_TextureMap[filename];
		}
		Ref<Texture2D> tex = Texture2D::Create(filename);
		m_TextureMap[filename] = tex;
	}

}