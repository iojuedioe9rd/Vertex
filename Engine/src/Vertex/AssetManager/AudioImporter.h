#pragma once
#include "Vertex/Audio/Audio.h"
#include "AssetMetadata.h"
#include "Asset.h"

namespace Vertex
{
	class VERTEX_API AudioImporter
	{
	public:
		static Ref<Audio> ImportAudio(AssetHandle handle, const AssetMetadata& metadata);

		static Ref<Audio> LoadAudio(const std::filesystem::path& path);
	};
	
}