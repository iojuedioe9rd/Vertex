#include "ThumbnailCache.h"

#include "Vertex/AssetManager/TextureImporter.h"
#include <chrono>
#include <Vertex/Core/Random.h>


namespace Vertex
{
	ThumbnailCache::ThumbnailCache(std::filesystem::path& path)
		: m_path(path)
	{
		m_ThumbnailCachePath = path / "Thumbnail.cache";
	}

	Ref<Texture2D> ThumbnailCache::GetOrCreateThumbnail(const std::filesystem::path& assetPath)
	{
		// 1. Read file timestamp
		// 2. Compare hashed timestamp with existing cached image (in memory first, then from cache file)
		// 3. If equal, return associated thumbnail, otherwise load asset from disk and generate thumbnail
		// 4. If generated new thumbnail, store in cache obviously

		auto absolutePath = std::filesystem::absolute(assetPath);
		std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(absolutePath);
		uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();

		if (m_CachedImages.find(assetPath) != m_CachedImages.end())
		{
			auto& cachedImage = m_CachedImages.at(assetPath);
			if (cachedImage.Timestamp == timestamp)
				return cachedImage.Image;
		}

		// TODO: PNGs for now
		if (assetPath.extension() != ".png")
			return nullptr;

		Ref<Texture2D> texture = TextureImporter::LoadTexture2D(absolutePath);
		if (!texture)
			return nullptr;

		texture->Resize(255 * (Random::Float() * 2.55f), 255 * (Random::Float() * 2.55f), true);

		auto& cachedImage = m_CachedImages[assetPath];
		cachedImage.Timestamp = timestamp;
		cachedImage.Image = texture;
		return cachedImage.Image;
	}
}