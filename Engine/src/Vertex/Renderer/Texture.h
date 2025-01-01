#pragma once

#include <string>

#include "Vertex/Core/Base.h"

namespace Vertex {

	class VERTEX_API Texture
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class VERTEX_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		static Ref<Texture2D> CreateWin(int resID, const std::string& format);
	};

	class VERTEX_API Texture2DAnimated : public Texture2D
	{
	public:
		static Ref<Texture2DAnimated> Create(uint32_t width, uint32_t height, uint16_t frames);
		//static Ref<Texture2DAnimated> Create(const std::string& path);

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