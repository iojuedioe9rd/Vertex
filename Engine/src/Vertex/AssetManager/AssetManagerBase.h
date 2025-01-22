#pragma once
#include <Vertex/Core/Base.h>
#include <Vertex/Core/UUID.h>
#include "Vertex/AssetManager/Asset.h"

namespace Vertex
{
	
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	class VERTEX_API AssetManagerBase
	{
	public:
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) const = 0;
		virtual bool IsAssetHandleValid(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;

	private:

	};

	
}