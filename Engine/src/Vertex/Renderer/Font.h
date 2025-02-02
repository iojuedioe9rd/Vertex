#pragma once


#include <filesystem>

#include "Vertex/Core/Base.h"
#include "Vertex/Renderer/Texture.h"
#include "Vertex/AssetManager/Asset.h"

namespace msdfgen
{
	class FontHandle;
}

namespace Vertex
{
	struct MSDFData;

	class VERTEX_API Font : public Asset
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }

		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();

		virtual AssetType GetType() const override { return AssetType::Font; }

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}