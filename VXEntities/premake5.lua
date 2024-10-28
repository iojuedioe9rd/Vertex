project "VXEntities"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
	cppdialect "C++17"

    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
		"src/**.asm",
	
        "src/**.h",
		"src/**.c",
		
		"src/**.hpp",
		"src/**.cpp"
    }

    includedirs
    {
		"%{wks.location}/Engine/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.mono}"
    }
	
	links
	{
		"yaml-cpp",
		"Engine",
		"Box2D",
		"%{Library.mono}"
	}
    
    filter "system:windows"
		systemversion "latest"
		links
		{
			"Ws2_32.lib",
			"Winmm.lib",
			"Version.lib",
			"Bcrypt.lib",
		}

	filter "configurations:Debug"
		defines "VX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VX_RELEASE"
		runtime "Release"
		symbols "on"
		optimize "on"

	filter "configurations:Dist"
		defines "VX_DIST"
		runtime "Release"
		symbols "off"
		optimize "full"