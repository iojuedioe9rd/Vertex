#pragma once


#include "Vertex/Renderer/Texture.h"
#include <glad/glad.h>

namespace Vertex {

	class VERTEX_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(int resID, const std::string& format);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Name;
		std::string m_Path = std::string();
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class VERTEX_API OpenGLTexture2DAnimated : public Texture2DAnimated
	{
	public:
		OpenGLTexture2DAnimated(uint32_t width, uint32_t height, uint16_t frames);
		OpenGLTexture2DAnimated(const std::string& path);
		//OpenGLTexture2DAnimated(int resID, const std::string& format);
		virtual ~OpenGLTexture2DAnimated();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererIDs[m_CurrentFrame]; }

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{

			return m_RendererIDs == ((OpenGLTexture2DAnimated&)other).m_RendererIDs;
		}

		virtual void SetFPS(float fps) override;
		virtual float GetFPS() override;

		virtual void Update(float dt) override;

		virtual void SetFrame(uint16_t frame) override;
		virtual uint16_t GetFrame() override { return m_CurrentFrame; }

		virtual void SetLooping(bool loop) override { m_Looping = loop; }
		virtual bool IsLooping() override { return m_Looping; }

		virtual void SetSpeed(float speed) override { m_Speed = speed; }
		virtual float GetSpeed() override { return m_Speed; }
	private:
		std::string m_Name;
		std::string m_Path = std::string();
		uint32_t m_Width, m_Height;
		uint32_t* m_RendererIDs;

		GLenum m_InternalFormat, m_DataFormat;
		
		const uint16_t m_Frames;
		float m_FPS = 60;
		uint16_t m_CurrentFrame = 0;
		bool m_Looping = false;
		float m_Speed = 1;

	};

}