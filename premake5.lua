include "Dependencies.lua"



workspace "Vertex"
	architecture "x86_64"
	startproject "Vertex-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Engine/vendor/Box2D"
include "Engine/vendor/ImGuizmo"
include "Engine/vendor/zlib"
include "Engine/vendor/yaml-cpp"
include "Engine/vendor/GLFW"
include "Engine/vendor/glad"
include "Engine/vendor/imgui"
include "Engine/vendor/checksum"
group ""


group "Core"
	include "Engine"
	include "Vertex-ScriptCore"
	include "VXEntities"
	include "Vertex-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""