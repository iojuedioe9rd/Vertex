#include <vxpch.h>
#include "Vertex/AssetManager/AssetManager.h"

namespace Vertex
{
	Ref<AssetManagerBase> AssetManager::s_AssetManagerBase = nullptr;

	AssetHandle AssetManager::GetAssetHandle(const std::filesystem::path& filepath)
	{
		return s_AssetManagerBase->GetAssetHandle(filepath);
	}

	Ref<Asset> AssetManager::GetAsset(AssetHandle handle)
	{
		return s_AssetManagerBase->GetAsset(handle);
	}

	bool AssetManager::IsAssetHandleValid(AssetHandle handle)
	{
		return s_AssetManagerBase->IsAssetHandleValid(handle);
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle)
	{
		return s_AssetManagerBase->IsAssetLoaded(handle);
	}

	AssetType AssetManager::GetAssetType(AssetHandle handle)
	{
		return s_AssetManagerBase->GetAssetType(handle);
	}

	void AssetManager::SetAssetManager(Ref<AssetManagerBase> assetManager)
	{
		s_AssetManagerBase = assetManager;
	}
}