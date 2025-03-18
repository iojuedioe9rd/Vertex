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
		EditorAssetManager(std::filesystem::path AssetRegistryPath);

		virtual Ref<Asset> GetAsset(AssetHandle handle) override;

		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual AssetType GetAssetType(AssetHandle handle) const override;

		virtual AssetHandle GetAssetHandle(const std::filesystem::path& filepath) const override;

		AssetHandle ImportAsset(const std::filesystem::path& filepath);
		

		const AssetMetadata& GetMetadata(AssetHandle handle) const;
		const std::filesystem::path& GetFilePath(AssetHandle handle) const;

		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }
		void SerializeAssetRegistry();
		bool DeserializeAssetRegistry();

		~EditorAssetManager() = default;
	private:
		std::filesystem::path AssetRegistryPath;

		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;

	};

	
}