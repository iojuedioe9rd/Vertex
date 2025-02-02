#pragma once
#include "Vertex/Renderer/Font.h"
#include "AssetMetadata.h"
#include "Asset.h"

namespace Vertex
{
	class VERTEX_API FontImporter
	{
	public:
		static Ref<Font> ImportFont(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Font> LoadFont(const std::filesystem::path& path);
	};

}