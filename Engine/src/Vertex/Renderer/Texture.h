#pragma once

#include <string>

#include "Vertex/AssetManager/Asset.h"
#include "Vertex/Core/Base.h"
#include "Vertex/Core/Buffer.h"

namespace Vertex {

	enum class VERTEX_API ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};
	struct VERTEX_API TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class VERTEX_API Texture : public Asset
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetData(Buffer* data) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class VERTEX_API Texture2D : public Texture
	{
	public:

		virtual void ToFile(std::filesystem::path filepath) const = 0;

		
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const TextureSpecification& specification, Buffer* data = nullptr);

		static Ref<Texture2D> CreateWin(int resID, const std::string& format);
		static AssetType GetStaticType() { return AssetType::Texture2D; }
		virtual AssetType GetType() const { return GetStaticType(); }
	};

	class VERTEX_API Texture2DAnimated : public Texture2D
	{
	public:
		/*
		static Ref<Texture2DAnimated> Create(uint32_t width, uint32_t height, uint16_t frames);
		static Ref<Texture2DAnimated> Create(const std::string& path);

		*/
		//static Ref<Texture2DAnimated> CreateWin(int resID, const std::string& format);

		virtual void SetFPS(float fps) = 0;
		virtual float GetFPS() = 0;

		virtual void Update(float dt) = 0;

		virtual void SetFrame(uint16_t frame) = 0;
		virtual uint16_t GetFrame() = 0;

		virtual void SetLooping(bool looping) = 0;
		virtual bool IsLooping() = 0;

		virtual void SetSpeed(float speed) = 0;
		virtual float GetSpeed() = 0;



	};

}