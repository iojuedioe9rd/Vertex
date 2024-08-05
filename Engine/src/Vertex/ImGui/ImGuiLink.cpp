#include "vxpch.h"
#include "ImGuiLink.h"
#include <imgui.h>

namespace Vertex
{
    bool ImGuiLink::Begin(char* name, bool* p_open, ImGuiWindowFlags flags)
    {
        return ImGui::Begin(name, p_open, flags);
    }


    void ImGuiLink::End()
    {
        ImGui::End();
    }


    bool ImGuiLink::ColorEdit3(char* name, float* value)
    {
        return ImGui::ColorEdit3(name, value);
    }

    void ImGuiLink::Text(const char* fmt)
    {
        ImGui::Text(fmt);
    }

    bool ImGuiLink::BeginMenuBar()
    {
        return ImGui::BeginMenuBar();
    }

    bool ImGuiLink::BeginMenu(char* name, bool e)
    {
        return ImGui::BeginMenu(name, e);
    }

    bool ImGuiLink::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
    {
        return ImGui::MenuItem(label, shortcut, selected, enabled);
    }

    void ImGuiLink::EndMenu()
    {
        ImGui::EndMenu();
    }

    void ImGuiLink::EndMenuBar()
    {
        ImGui::EndMenuBar();
    }

}

