#pragma once

#include <Vertex/Core/Base.h>
#include "Asset.h"
#include "AssetManagerBase.h"

namespace Vertex
{
    // Always declare the variable as extern
	class VERTEX_API AssetManager
	{
	public:
		static AssetHandle GetAssetHandle(const std::filesystem::path& filepath);
		static Ref<Asset> GetAsset(AssetHandle handle);
		static bool IsAssetHandleValid(AssetHandle handle);
		static bool IsAssetLoaded(AssetHandle handle);

		static AssetType GetAssetType(AssetHandle handle);

		static void SetAssetManager(Ref<AssetManagerBase> assetManager);

		static Ref<AssetManagerBase> GetAssetManager() { return s_AssetManagerBase; }

	private:
		static Ref<AssetManagerBase> s_AssetManagerBase;
	};

	
    
}
