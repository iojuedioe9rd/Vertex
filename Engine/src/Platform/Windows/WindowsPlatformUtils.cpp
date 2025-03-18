#include "vxpch.h"
#include "Vertex/Utils/PlatformUtils.h"

#include <sstream>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Vertex/Core/Application.h"
#include <stb_image.h>

namespace Vertex {

	// Convert narrow string to wide string
	static std::wstring StringToWideString(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
		std::wstring wideStr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wideStr[0], size_needed);
		return wideStr;
	}

	static unsigned char* LoadTextureFromResource(int resourceID, const char* format, int* width, int* height, int* channels) {
		// Get the handle to the current module
		HMODULE hModule = GetModuleHandle(NULL);

		// Convert format to wide string for FindResourceW
		std::wstring wideFormat = StringToWideString(format);

		// Use FindResourceW to find the resource
		HRSRC hResource = FindResourceW(hModule, MAKEINTRESOURCE(resourceID), wideFormat.c_str());
		if (!hResource) {
			std::cerr << "Failed to find resource!" << std::endl;
			return nullptr;
		}

		// Load the resource
		HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
		if (!hLoadedResource) {
			std::cerr << "Failed to load resource!" << std::endl;
			return nullptr;
		}

		// Get a pointer to the resource data
		void* pResourceData = LockResource(hLoadedResource);
		DWORD resourceSize = SizeofResource(hModule, hResource);
		if (!pResourceData || resourceSize == 0) {
			std::cerr << "Failed to lock resource!" << std::endl;
			return nullptr;
		}

		// Load the image using stbi_load_from_memory
		stbi_set_flip_vertically_on_load(1);
		unsigned char* imageData = stbi_load_from_memory(reinterpret_cast<unsigned char*>(pResourceData), resourceSize, width, height, channels, 0);
		if (!imageData) {
			std::cerr << "Failed to load image from memory!" << std::endl;
		}

		return imageData;
	}


	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[568] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[568] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	tex_id* TextureUtils::load_tex(const std::string& path, int* x, int* y, int* channels_in_file, int desired_channels)
	{
		
		return stbi_load(path.c_str(), x, y, channels_in_file, desired_channels);
	}

	tex_id* TextureUtils::load_tex_win_rc(int resourceID, const std::string& format, int* width, int* height, int* channels)
	{
		return LoadTextureFromResource(resourceID, format.c_str(), width, height, channels);
	}


	void TextureUtils::kill_tex(tex_id* tex)
	{
		stbi_image_free(tex);
	}

	Timestep Time::m_Timestep;
	float Time::GetTime()
	{
		return glfwGetTime();
	}
	/*
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	uint32_t counter = 0;
	*/

	float Time::FPS;
	
	float Time::GetFPS()
	{
		return FPS;
	}

	static struct PlatformDLLInstanceData
	{
		HMODULE hModule;
		std::fs::path dllpath;
	};

	DLLInstance::DLLInstance(const std::fs::path& path)
	{
		// Load the DLL
		PlatformDLLInstanceData* instance = new PlatformDLLInstanceData();
		instance->dllpath = path;
		instance->hModule = LoadLibrary(path.c_str());
		m_PlatformInstance = instance;
	}

	DLLInstance::~DLLInstance()
	{
		PlatformDLLInstanceData* data = static_cast<PlatformDLLInstanceData*>(m_PlatformInstance);
		if (data->hModule != nullptr)
		{
			FreeLibrary(data->hModule);
		}
		delete data;
	}

	bool DLLInstance::IsLoaded() const
	{
		PlatformDLLInstanceData* data = static_cast<PlatformDLLInstanceData*>(m_PlatformInstance);
		return data->hModule != nullptr;
	}
	

	std::string DLLInstance::GetName() const
	{
		PlatformDLLInstanceData* data = static_cast<PlatformDLLInstanceData*>(m_PlatformInstance);
		return data->dllpath.filename().string();
	}

	std::fs::path& DLLInstance::GetPath() const
	{
		PlatformDLLInstanceData* data = static_cast<PlatformDLLInstanceData*>(m_PlatformInstance);
		return data->dllpath;
	}

	void* DLLInstance::GetFunction(const std::string& functionName)
	{
		PlatformDLLInstanceData* data = static_cast<PlatformDLLInstanceData*>(m_PlatformInstance);
		if (data->hModule != nullptr)
		{
			return GetProcAddress(data->hModule, functionName.c_str());
		}
		return nullptr;
	}

	// unsigned char* LoadTextureFromResource(int resourceID, const char* format, int* width, int* height, int* channels)
}
