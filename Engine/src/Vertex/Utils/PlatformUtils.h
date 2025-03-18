#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/Core/Timestep.h"
#include <string>



typedef unsigned char tex_id;
// 568

namespace Vertex {

	class VERTEX_API FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class VERTEX_API TextureUtils
	{
	public:
		static tex_id* load_tex(const std::string& path, int* x, int* y, int* channels_in_file, int desired_channels);
		static tex_id* load_tex_win_rc(int resourceID, const std::string& format, int* width, int* height, int* channels);

		static void kill_tex(tex_id* tex);
	};

	class VERTEX_API Time
	{
	public:
		static float GetTime();

		static Timestep& GetTS()
		{
			return m_Timestep;
		}
		static float GetFPS();
		
	private:
		static Timestep m_Timestep;

		static float FPS;



		friend class Application;
		
	};

	class VERTEX_API DLLInstance
	{

	public:
		DLLInstance(const std::fs::path& path);
		~DLLInstance();

		
		bool IsLoaded() const;
		std::string GetName() const;
		std::fs::path& GetPath() const;

		// Add a template to cast function pointer
		template <typename FuncType>
		FuncType GetFunctionPointer(std::string functionName)
		{
			void* funcPtr = GetFunction(functionName);
			return funcPtr ? reinterpret_cast<FuncType>(funcPtr) : nullptr;
		}

	private:
		void* m_PlatformInstance; // Platform-specific instance (opaque pointer)
		void* GetFunction(const std::string& functionName);
	
	};
}