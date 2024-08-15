project "Checksum"
    kind "SharedLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/**.h"
        
    }

    includedirs
    {
        "src"
    }
    
    filter "system:windows"
        systemversion "latest"
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../../../bin/" .. outputdir .. "/Sandbox"),
			("{COPY} %{cfg.buildtarget.relpath} ../../../bin/" .. outputdir .. "/Vertex-Editor")
		}
		defines
		{
			"VX_BUILD_DLL_CHECKSUM",
			"GLFW_INCLUDE_NONE"
		}


    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "on"
        optimize "on"
    filter "configurations:Dist"
        runtime "Release"
        symbols "off"
        optimize "full"