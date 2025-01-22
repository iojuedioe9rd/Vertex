#pragma once
#include "AssetMetadata.h"

namespace Vertex {

	class VERTEX_API AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};
}