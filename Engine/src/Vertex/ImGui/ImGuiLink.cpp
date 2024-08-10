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

    bool ImGuiLink::ColorEdit4(char* name, float* value)
    {
        return ImGui::ColorEdit4(name, value);
    }

    void ImGuiLink::Text(const char* fmt, ...)
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
    //std::function<void()> func
    void ImGuiLink::EndMenuBar()
    {
        ImGui::EndMenuBar();
    }

    void ImGuiLink::Docking(bool dockingEnabled, std::function<void()> callback)
    {
        
        if (dockingEnabled)
        {
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            int window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            callback();

            ImGui::End();
        }
    }

    ImVec2 Vec2ToImVec2(glm::vec2 v)
    {
        return ImVec2{ v.x, v.y };
    }

    void ImGuiLink::Image(void* imageID, glm::vec2 size, glm::vec2 uv0, glm::vec2 uv1)
    {
        
        ImGui::Image((void*)imageID, Vec2ToImVec2(size), Vec2ToImVec2(uv0), Vec2ToImVec2(uv1));
    }

    

}

