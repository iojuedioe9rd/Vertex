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

		const MSDFData* GetMSDFData() const { return m_Data; }

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}