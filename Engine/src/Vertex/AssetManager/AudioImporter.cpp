#include "vxpch.h"
#include "AudioImporter.h"
#include "Vertex/Audio/AudioManager.h"


namespace Vertex
{
	Ref<Audio> AudioImporter::ImportAudio(AssetHandle handle, const AssetMetadata& metadata)
	{
		return AudioImporter::LoadAudio(std::filesystem::current_path() / metadata.FilePath);
	}
	Ref<Audio> AudioImporter::LoadAudio(const std::filesystem::path& path)
	{
		return Ref<Audio>(AudioManager::GetAudioFromFileName(path));
	}
	
}