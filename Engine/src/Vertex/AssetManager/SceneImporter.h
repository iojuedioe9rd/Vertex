#pragma once
#include "Asset.h"
#include "AssetMetadata.h"
#include "Vertex/Scene/Scene.h"
namespace Vertex {
	class VERTEX_API SceneImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetadata& metadata);

		// Load from filepath
		static Scene* LoadScene(const std::filesystem::path& path);
		static void SaveScene(Scene* scene, const std::filesystem::path& path);
	};
}