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
			case ImageFormat::RGBA32F: return GL_RGBA;
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
			case ImageFormat::RGBA32F: return GL_RGBA32F;
			}
			VX_CORE_ASSERT(false);
			return 0;
		}

	}

#define NEW_GL_TEX(tex, InternalFormat, Width, Height, DataFormat, DataType) \
    glCreateTextures(GL_TEXTURE_2D, 1, &tex); \
    glBindTexture(GL_TEXTURE_2D, tex); \
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, DataFormat, DataType, nullptr); \
    \
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR); \
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
    \
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT); \
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);

	

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
		m_DataType = GL_UNSIGNED_BYTE;
		NEW_GL_TEX(m_RendererID, m_InternalFormat, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE);

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

		m_DataType = (m_Specification.Format == ImageFormat::RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;

		NEW_GL_TEX(m_RendererID, m_InternalFormat, m_Width, m_Height, m_DataFormat, m_DataType);

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
		m_DataType = (m_Specification.Format == ImageFormat::RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		NEW_GL_TEX(m_RendererID, m_InternalFormat, m_Width, m_Height, m_DataFormat, m_DataType);

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

		//VX_CORE_ASSERT(success, "Failed to write texture to file: " + filepath.string());
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
		bool IS_FLOAT = m_DataType == GL_FLOAT;
		// (m_DataFormat == GL_RGBA32F) ? 16 : (m_DataFormat == GL_RGBA ? 4 : 3)


		uint32_t bpp;
		if (IS_FLOAT)
		{
			bpp = 16;
		}
		else
		{
			bpp = (m_DataFormat == GL_RGBA32F) ? 16 : (m_DataFormat == GL_RGBA ? 4 : 3);
		}
		uint32_t expectedSize = m_Width * m_Height * bpp;
		VX_CORE_ASSERT(data->Size == expectedSize, std::string("Data must be entire texture! Expected: ") + std::to_string(expectedSize));

		GLenum type = (m_DataFormat == GL_RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, type, data->Data);
	}


	void OpenGLTexture2D::Resize(uint32_t newWidth, uint32_t newHeight, bool aspect_ratio)
	{
		// Check if size is changing
		if (newWidth == m_Width && newHeight == m_Height) {
			std::cout << "Texture size is already the same, no resize needed." << std::endl;
			return;
		}

		glm::ivec2 size = glm::ivec2{ newWidth, newHeight };

		if (aspect_ratio) {
			if (m_Width != 0 && m_Height != 0) {
				float currentAspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

				// Scale the new size to fit inside the target size, maintaining the aspect ratio
				if (static_cast<float>(newWidth) / newHeight > currentAspectRatio) {
					// Fit to height
					size.y = newHeight;
					size.x = static_cast<int>(newHeight * currentAspectRatio);
				}
				else {
					// Fit to width
					size.x = newWidth;
					size.y = static_cast<int>(newWidth / currentAspectRatio);
				}
			}
		}

		// Ensure new size is valid
		if (newWidth == 0 || newHeight == 0) {
			std::cerr << "Invalid texture size: " << newWidth << "x" << newHeight << std::endl;
			return;
		}

		// Update the texture size
		newWidth = static_cast<uint32_t>(size.x);
		newHeight = static_cast<uint32_t>(size.y);

		std::cout << "Resizing texture to: " << newWidth << "x" << newHeight << std::endl;

		uint32_t newTex2D;
		m_DataType = (m_Specification.Format == ImageFormat::RGBA32F) ? GL_FLOAT : GL_UNSIGNED_BYTE;
		NEW_GL_TEX(newTex2D, m_InternalFormat, newWidth, newHeight, m_DataFormat, m_DataType);

		// Check for any OpenGL errors before framebuffer generation
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cerr << "OpenGL error before glGenFramebuffers: " << std::endl;
		}

		GLuint fboIds[2]{ 0 };

		// Ensure no framebuffers are bound before generating new ones
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind any active framebuffer

		glGenFramebuffers(2, fboIds);

		// Check if there were any OpenGL errors after generating framebuffers
		err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cerr << "OpenGL error after glGenFramebuffers: " << std::endl;
			return;
		}

		std::cout << "Generated framebuffers with IDs: " << fboIds[0] << " and " << fboIds[1] << std::endl;

		// Attach the old texture to the read framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboIds[0]);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererID, 0);

		// Attach the new texture to the draw framebuffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboIds[1]);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, newTex2D, 0);

		// Perform the blit operation to center the texture
		int xOffset = (newWidth - size.x) / 2;
		int yOffset = (newHeight - size.y) / 2;
		glBlitFramebuffer(0, 0, m_Width, m_Height, xOffset, yOffset, xOffset + size.x, yOffset + size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// Check if there were any OpenGL errors during the blit operation
		err = glGetError();
		if (err != GL_NO_ERROR) {
			
		}

		std::cout << "Blit completed." << std::endl;

		// Clean up the old texture and framebuffer
		glDeleteTextures(1, &m_RendererID);
		glDeleteFramebuffers(2, fboIds);

		// Update the texture reference and size
		m_RendererID = newTex2D;
		m_Width = newWidth;
		m_Height = newHeight;

		m_Specification.Width = newWidth;
		m_Specification.Height = newHeight;

		std::cout << "Texture resized successfully." << std::endl;
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