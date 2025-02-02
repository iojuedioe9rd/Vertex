#include <vxpch.h>
#include "FontImporter.h"

namespace Vertex
{
	Ref<Font> FontImporter::ImportFont(AssetHandle handle, const AssetMetadata& metadata)
	{
		return FontImporter::LoadFont(std::filesystem::current_path() / metadata.FilePath);
	}
	Ref<Font> FontImporter::LoadFont(const std::filesystem::path& path)
	{
		return CreateRef< Font>(path);
	}
}