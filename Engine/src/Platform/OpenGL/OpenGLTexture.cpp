#include "vxpch.h"
#include "OpenGLTexture.h"
#include "Vertex/Utils/PlatformUtils.h"
#include <stb_image_write.h>

#include "stb_image.h"

#include <glad/glad.h>

#include "Vertex/Animation/AnimationLoader.h"

#define READ_TEX_PIXEL(Y, X, FRAME, OFFSET, WIDTH, HEIGHT) \
    (y * HEIGHT * 3 + x * 3)

#define FREE_2_BUTTER(A, B) \
{\
	free(A);\
	free(B);\
}\

namespace Vertex 
{
	namespace Utils {
		static GLenum VertexImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}
			VX_CORE_ASSERT(false);
			return 0;
		}

		static GLenum VertexImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}
			VX_CORE_ASSERT(false);
			return 0;
		}
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

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification, Buffer* data)
		:m_Width(specification.Width), m_Height(specification.Height)
	{
		VX_PROFILE_FUNCTION();

		m_Specification = specification;

		m_Width = specification.Width;
		m_Height = specification.Height;

		m_InternalFormat = Utils::VertexImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::VertexImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data)
			SetData(data);
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

	void OpenGLTexture2D::ToFile(std::filesystem::path filepath) const
	{
		// Determine bytes per pixel based on the data format
		uint32_t bpp = (m_DataFormat == GL_RGBA) ? 4 : 3;
		size_t dataSize = m_Width * m_Height * bpp;

		// Allocate memory to store the texture data
		uint8_t* data = (uint8_t*)malloc(dataSize);
		VX_CORE_ASSERT(data, "Failed to allocate memory for texture data!");

		// Retrieve texture data from OpenGL
		glGetTextureImage(m_RendererID, 0, m_DataFormat, GL_UNSIGNED_BYTE, dataSize, data);

		// Flip the texture vertically
		uint8_t* flippedData = (uint8_t*)malloc(dataSize);
		VX_CORE_ASSERT(flippedData, "Failed to allocate memory for flipped texture data!");

		for (int row = 0; row < m_Height; ++row)
		{
			memcpy(flippedData + row * m_Width * bpp,
				data + (m_Height - row - 1) * m_Width * bpp,
				m_Width * bpp);
		}

		// Write the flipped image to a file using stb_image_write
		bool success = false;
		std::string extension = filepath.extension().string();

		if (extension == ".png")
		{
			success = stbi_write_png(filepath.string().c_str(), m_Width, m_Height, bpp, flippedData, m_Width * bpp);
		}
		else if (extension == ".jpg" || extension == ".jpeg")
		{
			success = stbi_write_jpg(filepath.string().c_str(), m_Width, m_Height, bpp, flippedData, 90); // Quality: 90
		}
		else if (extension == ".bmp")
		{
			success = stbi_write_bmp(filepath.string().c_str(), m_Width, m_Height, bpp, flippedData);
		}
		else if (extension == ".tga")
		{
			success = stbi_write_tga(filepath.string().c_str(), m_Width, m_Height, bpp, flippedData);
		}
		else
		{
			FREE_2_BUTTER(data, flippedData);
			VX_CORE_ASSERT(false, "Unsupported file format: " + extension);
		}

		// Free allocated memory
		if(data != nullptr) { FREE_2_BUTTER(data, flippedData); }

		VX_CORE_ASSERT(success, "Failed to write texture to file: " + filepath.string());
	}


	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		uint32_t expectedSize = m_Width * m_Height * bpp;
		VX_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!" + std::to_string(expectedSize));
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::SetData(Buffer* data)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		uint32_t expectedSize = m_Width * m_Height * bpp;
		VX_CORE_ASSERT(data->Size == m_Width * m_Height * bpp, "Data must be entire texture! " + std::to_string(expectedSize));
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data->Data);
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