#pragma once
#include "Vertex/Core/Base.h"
#include <glm/glm.hpp>

namespace Vertex
{
    

	class VERTEX_API ImGuiLink
	{


	public:

        enum ImGuiWindowFlags
        {
            ImGuiWindowFlags_None = 0,
            ImGuiWindowFlags_NoTitleBar = 1 << 0,
            ImGuiWindowFlags_NoResize = 1 << 1,
            ImGuiWindowFlags_NoMove = 1 << 2,
            ImGuiWindowFlags_NoScrollbar = 1 << 3,
            ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,
            ImGuiWindowFlags_NoCollapse = 1 << 5,
            ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
            ImGuiWindowFlags_NoBackground = 1 << 7,
            ImGuiWindowFlags_NoSavedSettings = 1 << 8,
            ImGuiWindowFlags_NoMouseInputs = 1 << 9,
            ImGuiWindowFlags_MenuBar = 1 << 10,
            ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,
            ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,
            ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,
            ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
            ImGuiWindowFlags_NoNavInputs = 1 << 16,
            ImGuiWindowFlags_NoNavFocus = 1 << 17,
            ImGuiWindowFlags_UnsavedDocument = 1 << 18,
            ImGuiWindowFlags_NoNav = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
            ImGuiWindowFlags_NoDecoration = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
            ImGuiWindowFlags_NoInputs = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
            ImGuiWindowFlags_NavFlattened = 1 << 23,
            ImGuiWindowFlags_ChildWindow = 1 << 24,
            ImGuiWindowFlags_Tooltip = 1 << 25,
            ImGuiWindowFlags_Popup = 1 << 26,
            ImGuiWindowFlags_Modal = 1 << 27,
            ImGuiWindowFlags_ChildMenu = 1 << 28

        };
		static bool Begin(char* name, bool* p_open = (bool*)(NULL), ImGuiWindowFlags flags = ImGuiWindowFlags::ImGuiWindowFlags_None);
		static void End();
		static bool ColorEdit3(char* name, float* value);
        static bool ColorEdit4(char* name, float* value);
        static void Text(const char* fmt, ...);
        static bool BeginMenuBar();
        static bool BeginMenu(char* name, bool e = true);
        static bool MenuItem(const char* label, const char* shortcut = (const char*)0, bool selected = false, bool enabled = true);
        static void EndMenu();
        static void EndMenuBar();
        static void Docking(bool dockingEnabled = true, std::function<void()> callback = NULL);
        static void Image(void* imageID, glm::vec2 size, glm::vec2 uv0 = glm::vec2(0,0), glm::vec2 uv1 = glm::vec2(1,1));
        static bool ImageButtonWithText(void* texture, const std::string& text, glm::vec2 size, int numButtons = 1, glm::vec2 padding = glm::vec2(0,0), glm::vec2 uv0 = glm::vec2(0, 0), glm::vec2 uv1 = glm::vec2(1, 1));
        static void SameLine();
	};

}

