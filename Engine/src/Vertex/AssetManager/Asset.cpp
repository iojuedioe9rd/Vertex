#include "vxpch.h"
#include "Vertex/AssetManager/Asset.h"

namespace Vertex
{
	std::string_view AssetTypeToString(AssetType type)
	{
		switch (type)
		{
		case AssetType::None:      return "AssetType::None";
		case AssetType::Scene:     return "AssetType::Scene";
		case AssetType::Texture2D: return "AssetType::Texture2D";
		case AssetType::Audio: return "AssetType::Audio";
		case AssetType::Font: return "AssetType::Font";
		}
		return "AssetType::<Invalid>";
	}

	AssetType AssetTypeFromString(std::string_view assetType)
	{
		if (assetType == "AssetType::None")      return AssetType::None;
		if (assetType == "AssetType::Scene")     return AssetType::Scene;
		if (assetType == "AssetType::Texture2D") return AssetType::Texture2D;
		if (assetType == "AssetType::Audio") return AssetType::Audio;
		if (assetType == "AssetType::Font") return AssetType::Font;
		return AssetType::None;
	}
}