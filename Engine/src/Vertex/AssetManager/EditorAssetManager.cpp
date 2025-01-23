#include <vxpch.h>
#include "Vertex/AssetManager/EditorAssetManager.h"

#include "AssetImporter.h"

#include <yaml-cpp/yaml.h>

namespace Vertex
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const std::string_view& v)
	{
		out << std::string(v.data(), v.size());
		return out;
	}

	bool EditorAssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	AssetHandle EditorAssetManager::GetAssetHandle(const std::filesystem::path& filepath) const
	{
		 for (auto& metadata : m_AssetRegistry)
		 {
			 if (metadata.second.FilePath == filepath)
			 {
				 return metadata.first;
			 }
		 }

		 return 0;
	}

	AssetHandle EditorAssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		if (IsAssetHandleValid(GetAssetHandle(filepath)))
		{
			return GetAssetHandle(filepath);
		}

		AssetHandle handle; // generate new handle
		AssetMetadata metadata;
		metadata.FilePath = filepath;
		metadata.Type = AssetType::Texture2D; // TODO: grab this from extension and try to load
		Ref<Asset> asset = AssetImporter::ImportAsset(handle, metadata);
		asset->Handle = handle;
		if (asset)
		{
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = metadata;
			SerializeAssetRegistry();
		}

		return handle;
	}

	

	void EditorAssetManager::SerializeAssetRegistry()
	{
		
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;
			out << YAML::BeginSeq;
			for (const auto& [handle, metadata] : m_AssetRegistry)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				std::string filepathStr = metadata.FilePath.generic_string();
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(metadata.Type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}
		std::ofstream fout(AssetRegistryPath);
		fout << out.c_str();
	}
	bool EditorAssetManager::DeserializeAssetRegistry()
	{
		
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(AssetRegistryPath.string());
		}
		catch (YAML::ParserException e)
		{
			VX_CORE_ERROR("Failed to load project file '{0}'\n     {1}", AssetRegistryPath.string(), e.what());
			return false;
		}
		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;
		for (const auto& node : rootNode)
		{
			AssetHandle handle = node["Handle"].as<uint64_t>();
			auto& metadata = m_AssetRegistry[handle];
			metadata.FilePath = node["FilePath"].as<std::string>();
			metadata.Type = AssetTypeFromString(node["Type"].as<std::string>());

			m_AssetRegistry[handle] = metadata;
		}
		return true;
	}

	const AssetMetadata& EditorAssetManager::GetMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return s_NullMetadata;
		return it->second;
	}

	EditorAssetManager::EditorAssetManager(std::filesystem::path AssetRegistryPath)
	{
		this->AssetRegistryPath = AssetRegistryPath;
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle) const
	{
		// 1. check if handle is valid
		if (!IsAssetHandleValid(handle))
			return nullptr;
		// 2. check if asset needs load (and if so, load)
		Ref<Asset> asset;
		if (IsAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			// load asset
			const AssetMetadata& metadata = GetMetadata(handle);
			asset = AssetImporter::ImportAsset(handle, metadata);
			if (!asset)
			{
				// import failed
				VX_CORE_ERROR("EditorAssetManager::GetAsset - asset import failed!");
			}
		}
		// 3. return asset
		return asset;
	}
}