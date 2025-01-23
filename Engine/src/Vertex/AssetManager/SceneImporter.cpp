#include <vxpch.h>

#include <Vertex/AssetManager/SceneImporter.h>

#include "Vertex/Scene/SceneSerializer.h"

namespace Vertex {
	Ref<Scene> SceneImporter::ImportScene(AssetHandle handle, const AssetMetadata& metadata)
	{
		VX_PROFILE_FUNCTION();

		return Ref<Scene>(LoadScene(metadata.FilePath));
	}
	Scene* SceneImporter::LoadScene(const std::filesystem::path& path)
	{
		VX_PROFILE_FUNCTION();
		Scene* scene = new Scene(path.filename().string().c_str());
		SceneSerializer serializer(&scene);
		serializer.Deserialize(path.string());
		return scene;
	}
	void SceneImporter::SaveScene(Scene* scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(&scene);
		serializer.Serialize(path.string());
	}
}