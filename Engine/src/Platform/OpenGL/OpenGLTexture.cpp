#include "vxpch.h"
#include "OpenGLTexture.h"
#include "Vertex/Utils/PlatformUtils.h"

#include "stb_image.h"

#include <glad/glad.h>

#include "Vertex/Animation/AnimationLoader.h"

#define READ_TEX_PIXEL(Y, X, FRAME, OFFSET, WIDTH, HEIGHT) \
    (y * HEIGHT * 3 + x * 3)


namespace Vertex 
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path), m_Name(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		VX_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		m_InternalFormat = 0;
		m_DataFormat = 0;
		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}

		VX_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(int resID, const std::string& format) :
		m_Name("App resource ID: " + std::to_string(resID) + " , Format: " + format)
	{
		int width, height, channels;
		stbi_uc* data = (stbi_uc*)TextureUtils::load_tex_win_rc(resID, format, &width, &height, &channels);

		VX_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		m_InternalFormat = 0;
		m_DataFormat = 0;
		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}

		VX_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		TextureUtils::kill_tex((tex_id*)data);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		VX_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	//
	//		O P E N G L   T E X T U R E   2 D   A N I M A T E D
	//

	OpenGLTexture2DAnimated::OpenGLTexture2DAnimated(uint32_t width, uint32_t height, uint16_t frames) : m_Frames(frames)
	{
		m_CurrentFrame = 0;
		m_Width = width;
		m_Height = height;
		m_RendererIDs = new uint32_t[m_Frames];

		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RGB;

        uint8_t** data = new uint8_t*[frames];

		for (uint16_t i = 1; i <= frames; i++)
		{
			data[i - 1] = new uint8_t[m_Width * m_Height * 3];
			uint8_t* frameData = data[i - 1];

			VX_EXTRA_CORE_TRACE("Loading frame: {0}", i);
			for (uint32_t y = 0; y < height; ++y)
			{
				for (uint32_t x = 0; x < width; ++x)
				{
					size_t base_index = READ_TEX_PIXEL(y, x, i - 1, 0, width, height);
					frameData[base_index + 0] = ((width + height * x * y) * i * 1) % 256; // Red
					frameData[base_index + 1] = ((width + height * x * y) * i * 2) % 256; // Green
					frameData[base_index + 2] = ((width + height * x * y) * i * 3) % 256; // Blue
				}
			}

			data[i - 1] = frameData;
		}

		// Generate and initialize textures
		glCreateTextures(GL_TEXTURE_2D, m_Frames, m_RendererIDs);

		for (uint16_t frame = 0; frame < m_Frames; frame++) {
			VX_EXTRA_CORE_TRACE("Initializing texture for frame: {0}", frame + 1);

			uint32_t textureID = m_RendererIDs[frame];

			// Allocate immutable storage for each texture
			glTextureStorage2D(textureID, 1, m_InternalFormat, m_Width, m_Height);

			// Set texture parameters
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Upload data to the texture
			glTextureSubImage2D(textureID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data[frame]);

			// Free the frame data after uploading
			delete[] data[frame];
		}

		// Clean up allocated data array
		delete[] data;
	}

	OpenGLTexture2DAnimated::OpenGLTexture2DAnimated(const std::string& path)
	{
		//m_InternalFormat = GL_RGB8;
		//m_DataFormat = GL_RGB;

		//Video m_Video;
		//glm::ivec3 m_Info;
		//AnimationLoader::video_reader_open(&m_Video, &m_Info, path.c_str());
		
		//m_Frames = m_Info.z;
		//m_Width = m_Info.x;
		//m_Height = m_Info.y;

		//VX_INFO("Frames: {0}, Width: {1}, Height: {2}", m_Frames, m_Width, m_Height);

	}

	OpenGLTexture2DAnimated::~OpenGLTexture2DAnimated()
	{
		glDeleteTextures(m_Frames, m_RendererIDs);
		delete[] m_RendererIDs;
	}


	void OpenGLTexture2DAnimated::SetData(void* data, uint32_t size)
	{
	}

	void OpenGLTexture2DAnimated::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererIDs[m_CurrentFrame]);
	}


	void OpenGLTexture2DAnimated::SetFPS(float fps)
	{
		m_FPS = fps;
	}

	float OpenGLTexture2DAnimated::GetFPS()
	{
		return m_FPS;
	}

	void OpenGLTexture2DAnimated::Update(float dt)
	{
		static float elapsedTime = 0.0f;
		elapsedTime += dt * m_Speed;

		if (elapsedTime >= (1.0f / m_FPS))
		{
			elapsedTime = 0.0f;
			m_CurrentFrame = (m_CurrentFrame + 1) % m_Frames;
			if (!m_Looping && m_CurrentFrame == 0)
				m_CurrentFrame = m_Frames - 1; // Stay on the last frame
		}
	}

	void OpenGLTexture2DAnimated::SetFrame(uint16_t frame)
	{
		if (frame < m_Frames)
		{
			m_CurrentFrame = frame;
		}
	}
}