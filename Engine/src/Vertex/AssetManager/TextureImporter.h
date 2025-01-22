#pragma once

#include "Asset.h"
#include "AssetMetadata.h"

#include "Vertex/Renderer/Texture.h"
namespace Vertex {
	class VERTEX_API TextureImporter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);
	};
}