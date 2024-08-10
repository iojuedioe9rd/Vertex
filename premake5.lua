include "Dependencies.lua"

workspace "Vertex"
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
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Engine/vendor/zlib"
include "Engine/vendor/GLFW"
include "Engine/vendor/glad"
include "Engine/vendor/imgui"
include "Engine/vendor/checksum"
group ""


group "Core"
	include "Engine"
group ""

group "Misc"
	include "Sandbox"
	include "Engine/vendor/VertCLI"
group ""