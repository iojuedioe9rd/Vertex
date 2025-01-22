#pragma once
#include <Vertex/Core/Base.h>

#include "AssetManagerBase.h"
#include "AssetMetadata.h"


namespace Vertex
{
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class VERTEX_API EditorAssetManager : public AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) const override;
		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		const AssetMetadata& GetMetadata(AssetHandle handle) const;
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;

	};

	
}