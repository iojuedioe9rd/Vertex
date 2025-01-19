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
IncludeDir["filewatch"] = "%{wks.location}/Engine/vendor/filewatch"
IncludeDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg/include"
IncludeDir["msdfgen"] = "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Engine/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Engine/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg/lib"
LibraryDir["ffmpeg_cpp"] = "%{wks.location}/Engine/vendor/ffmpeg-cpp/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["ffmpegAvcodec"] = "%{LibraryDir.ffmpeg}/avcodec.lib"
Library["ffmpegAvdevice"] = "%{LibraryDir.ffmpeg}/avdevice.lib"
Library["ffmpegAvfilter"] = "%{LibraryDir.ffmpeg}/avfilter.lib"
Library["ffmpegAvformat"] = "%{LibraryDir.ffmpeg}/avformat.lib"
Library["ffmpegAvutil"] = "%{LibraryDir.ffmpeg}/avutil.lib"

Library["ffmpegPostproc"] = "%{LibraryDir.ffmpeg}/postproc.lib"

Library["ffmpegSwresample"] = "%{LibraryDir.ffmpeg}/swresample.lib"
Library["ffmpegSwscale"] = "%{LibraryDir.ffmpeg}/swscale.lib"