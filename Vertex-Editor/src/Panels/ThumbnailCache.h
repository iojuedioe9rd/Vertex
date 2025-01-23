#pragma once


#include "Vertex/Renderer/Texture.h"

namespace Vertex {

	struct ThumbnailImage
	{
		uint64_t Timestamp;
		Ref<Texture2D> Image;
	};
	class ThumbnailCache
	{
	public:
		ThumbnailCache(std::filesystem::path& path);
		Ref<Texture2D> GetOrCreateThumbnail(const std::filesystem::path& path);
	private:
		std::filesystem::path m_path;
		std::map<std::filesystem::path, ThumbnailImage> m_CachedImages;

		// TEMP (replace with Hazel::Serialization)
		std::filesystem::path m_ThumbnailCachePath;
	};
}