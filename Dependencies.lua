VULKAN_SDK = os.getenv("VULKAN_SDK")

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
IncludeDir["lua"] = "%{wks.location}/Engine/vendor/lua"
IncludeDir["luawrapper"] = "%{wks.location}/Engine/vendor/luawrapper/include"
IncludeDir["boost"] = "%{wks.location}/Engine/vendor/boost"
IncludeDir["sol2"] = "%{wks.location}/Engine/vendor/sol2/single/include"
IncludeDir["rttr"] = "%{wks.location}/Engine/vendor/rttr/src"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Engine/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["OpenAL"] = "%{wks.location}/Engine/vendor/OpenAL/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Engine/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["ffmpeg"] = "%{wks.location}/Engine/vendor/ffmpeg/lib"
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
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

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"