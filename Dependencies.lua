IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/vendor/GLFW/include"
IncludeDir["glad"] = "%{wks.location}/Engine/vendor/glad/include"
IncludeDir["imgui"] = "%{wks.location}/Engine/vendor/imgui"
IncludeDir["VertexOopsFormat"] = "%{wks.location}/Engine/vendor/VertexOopsFormat/src"
IncludeDir["glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Engine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/Engine/vendor/Box2D/include"
IncludeDir["imguizmo"] = "%{wks.location}/Engine/vendor/ImGuizmo"
IncludeDir["mono"] = "%{wks.location}/Engine/vendor/mono/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Engine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"