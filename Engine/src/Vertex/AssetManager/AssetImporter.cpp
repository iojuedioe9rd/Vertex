#include "vxpch.h"
#include "AssetImporter.h"
#include "TextureImporter.h"
#include "SceneImporter.h"
#include "AudioImporter.h"
#include "FontImporter.h"

#include <map>
namespace Vertex {

	using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
	{ AssetType::Texture2D, [](AssetHandle handle, const AssetMetadata& metadata) {
		return TextureImporter::ImportTexture2D(handle, metadata);
	}},
	{ AssetType::Scene, [](AssetHandle handle, const AssetMetadata& metadata) {
		return SceneImporter::ImportScene(handle, metadata);
	}},
	{ AssetType::Audio, [](AssetHandle handle, const AssetMetadata& metadata) {
		return AudioImporter::ImportAudio(handle, metadata);
	}},
	{ AssetType::Font, [](AssetHandle handle, const AssetMetadata& metadata) {
		return FontImporter::ImportFont(handle, metadata);
	}}
	};

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
		{
			VX_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata.Type);
			return nullptr;
		}
		return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}
}