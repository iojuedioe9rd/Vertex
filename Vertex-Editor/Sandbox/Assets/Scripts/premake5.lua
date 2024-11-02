local VertexRootDir = '../../../..'
include (VertexRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

print("HI 1")
workspace "Sandbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

print("HI 2")
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

print("HI 3")
project "Sandbox"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	links
	{
		"Vertex-ScriptCore"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
print("HI 4")
group "Vertex"
	include (VertexRootDir .. "/Vertex-ScriptCore")
group ""
print("HI 5")