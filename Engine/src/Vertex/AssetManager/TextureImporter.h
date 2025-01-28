#pragma once

#include "Asset.h"
#include "Vertex/Renderer/Texture.h"
#include "AssetMetadata.h"




namespace Vertex {
	class VERTEX_API TextureImporter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	};
}