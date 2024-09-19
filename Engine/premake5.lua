project "Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vxpch.h"
	pchsource "src/vxpch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"src/**.c",
		"src/**.asm",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/miniaudio/**.h",
		"vendor/miniaudio/**.cpp",
		"vendor/simple_ecs/**.h",
		"vendor/simple_ecs/**.cpp",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	includedirs {
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.glm}",
		"vendor/checksum/src",
		"vendor/stb_image",
		"vendor/miniaudio",
		"%{IncludeDir.entt}",
		"vendor/simple_ecs"
		
	}
	
	links {
		"Glad",
		"opengl32.lib",
		"GLFW",
		"imgui",
		"zlib",
		"Checksum"
		
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"VX_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Vertex-Editor")
		}

		links
		{
			"winmm"
		}
		
	filter "configurations:Debug"
		defines "VX_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			
		}

	filter "configurations:Release"
		defines "VX_RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

		links
		{

		}

	filter "configurations:Dist"
		defines "VX_DIST"
		runtime "Release"
		symbols "off"
		optimize "full"

		links
		{
			
		}
	