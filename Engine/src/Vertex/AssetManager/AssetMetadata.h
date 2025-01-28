#pragma once
#include "Vertex/Core/Base.h"
#include <filesystem>
#include "Asset.h"

namespace Vertex {

	struct VERTEX_API AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;
		operator bool() const { return Type != AssetType::None; }
	};
}