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
		"vendor/simple_ecs",
		"%{IncludeDir.imguizmo}",
		"%{IncludeDir.ffmpeg}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.mono}"
	}
	
	links {
		"Glad",
		"opengl32.lib",
		"GLFW",
		"imgui",
		"zlib",
		"Checksum",
		"ImGuizmo",
		"msdf-atlas-gen",
		"msdfgen",
		"yaml-cpp",
		"Box2D",
		"%{Library.mono}"
		
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
			("{MKDIR} ../bin/" .. outputdir .. "/Sandbox"),
			("{MKDIR} ../bin/" .. outputdir .. "/Vertex-Editor"),
		
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Vertex-Editor"),
			
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
			
		}

		links
		{
			"Ws2_32.lib",
			"Winmm.lib",
			"Version.lib",
			"Bcrypt.lib",
			"d3d12.lib",
			"dxgi.lib",
			"d3dcompiler.lib"
		}
		
	filter "configurations:Debug"
		defines "VX_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}
		
	filter "configurations:Release"
		defines "VX_RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "VX_DIST"
		runtime "Release"
		symbols "off"
		optimize "speed"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
	