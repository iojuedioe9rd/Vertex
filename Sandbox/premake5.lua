project "Sandbox"
	kind "ConsoleApp"
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
	
	includedirs
	{
		"%{wks.location}/Engine/src",
		"%{IncludeDir.spdlog}"
	}
	
	links
	{
		"Engine"
	}
	
	filter "system:windows"
		systemversion "latest"

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