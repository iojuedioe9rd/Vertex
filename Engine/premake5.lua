project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	includedirs {
		"src"
	}
	
	links {
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		links
		{
			
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
	