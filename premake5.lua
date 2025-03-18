-- Include dependencies configuration
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
      "YAML_CPP_STATIC_DEFINE",
	  "MOJOSHADER_NO_VERSION_INCLUDE"
   }

   flags
   {
      "MultiProcessorCompile"
   }

   vectorextensions "SSE4.2"

   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

   -- Group Dependencies
   group "Dependencies"
      include "Engine/vendor/Box2D"
      include "Engine/vendor/ImGuizmo"
      include "Engine/vendor/zlib"
      include "Engine/vendor/yaml-cpp"
      include "Engine/vendor/GLFW"
      include "Engine/vendor/glad"
      include "Engine/vendor/imgui"
      include "Engine/vendor/checksum"
      include "Engine/vendor/msdf-atlas-gen"
      include "Engine/vendor/lua"
      include "Engine/vendor/rttr"
	  include "Engine/vendor/OpenAL"
   group "" -- End Dependencies group

   

   -- Group Core
   group "Core"
      include "Engine"
      include "Vertex-ScriptCore"
      include "Vertex-Editor"
   group "" -- End Core group

   -- Group Misc
   group "Misc"
      include "Sandbox"
   group "" -- End Misc group
