#pragma once
#include <Vertex/Core/Base.h>
#include <Vertex/Core/UUID.h>
#include "Vertex/AssetManager/Asset.h"
#include <filesystem> // Add this include to resolve std::filesystem

namespace Vertex
{
	
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	class VERTEX_API AssetManagerBase
	{
	public:
		virtual AssetHandle GetAssetHandle(const std::filesystem::path& filepath) const = 0;
		virtual Ref<Asset> GetAsset(AssetHandle handle) = 0;
		virtual bool IsAssetHandleValid(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;

		virtual AssetType GetAssetType(AssetHandle handle) const = 0;

	private:

	};

	
}