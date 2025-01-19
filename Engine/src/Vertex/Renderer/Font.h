#pragma once


#include <filesystem>

#include "Vertex/Core/Base.h"
#include "Vertex/Renderer/Texture.h"

namespace msdfgen
{
	class FontHandle;
}

namespace Vertex
{
	struct MSDFData;

	class VERTEX_API Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}