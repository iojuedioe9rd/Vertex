#include "vxpch.h"
#include "ImGuiLink.h"
#include <imgui.h>
#include <imgui_internal.h>

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

    glm::vec2 ImVec2ToVec2(ImVec2 v)
    {
        return glm::vec2{ v.x, v.y };
    }

    void ImGuiLink::Image(void* imageID, glm::vec2 size, glm::vec2 uv0, glm::vec2 uv1)
    {
        
        ImGui::Image((void*)imageID, Vec2ToImVec2(size), Vec2ToImVec2(uv0), Vec2ToImVec2(uv1));
    }

    bool ImGuiLink::ImageButton(void* imageID, glm::vec2 size, glm::vec2 uv0, glm::vec2 uv1)
    {
        return ImGui::ImageButton(imageID, Vec2ToImVec2(size), Vec2ToImVec2(uv0), Vec2ToImVec2(uv1));
    }

    void ImGuiLink::PushStyleVar(int styleVar, glm::vec2 idk)
    {
        ImGui::PushStyleVar(styleVar, Vec2ToImVec2(idk));
    }

    void ImGuiLink::PopStyleVar()
    {
        ImGui::PopStyleVar();
    }

    glm::vec2 ImGuiLink::GetContentRegionAvail()
    {
        return ImVec2ToVec2(ImGui::GetContentRegionAvail());
    }

    bool ImGuiLink::IsWindowFocused()
    {
        return ImGui::IsWindowFocused();
    }

    bool ImGuiLink::IsWindowHovered()
    {
        return  ImGui::IsWindowHovered();
    }

    void ImGuiLink::Separator()
    {
        ImGui::Separator();
    }

    void ImGuiLink::DragFloat3(std::string label, float* value, float speed)
    {
        ImGui::DragFloat3(label.c_str(), value, speed);
    }

    bool ImGuiLink::Checkbox(std::string label, bool* v)
    {
        return ImGui::Checkbox(label.c_str(), v);
    }

    bool ImGuiLink::TreeNodeEx(std::string srt_id, int Flags, std::string label)
    {
        return ImGui::TreeNodeEx(srt_id.c_str(), (ImGuiTreeNodeFlags)Flags, label.c_str());
    }

    bool ImGuiLink::TreeNodeEx(void* ptr_id, int Flags, std::string label)
    {
        return ImGui::TreeNodeEx(ptr_id, (ImGuiTreeNodeFlags)Flags, label.c_str());
    }

    bool ImGuiLink::IsItemClicked()
    {
        return ImGui::IsItemClicked();
    }

    void ImGuiLink::TreePop()
    {
        ImGui::TreePop();
    }

    bool ImGuiLink::InputText(char* sdd, char* buffer, size_t buffer_size)
    {
        return ImGui::InputText(sdd, buffer, buffer_size);
    }

    bool ImGuiLink::IsMouseDown(int mouseButton)
    {
        return ImGui::IsMouseDown(mouseButton);
    }

    bool ImGuiLink::BeginCombo(char* name, char* idk)
    {
        return ImGui::BeginCombo(name, idk);
    }

    void ImGuiLink::EndCombo()
    {
        ImGui::EndCombo();
    }

    bool ImGuiLink::Selectable(char* name, bool* selected)
    {
        return ImGui::Selectable(name, selected);
    }

    bool ImGuiLink::DragFloat(std::string label, float value_ptr[1])
    {
        return ImGui::DragFloat(label.c_str(), value_ptr);
    }

    void ImGuiLink::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool ImGuiLink::BeginPopupContextWindow()
    {
        return ImGui::BeginPopupContextWindow();
    }

    void ImGuiLink::EndPopup()
    {
        ImGui::EndPopup();
    }

    bool ImGuiLink::Button(std::string label)
    {
        return ImGui::Button(label.c_str());
    }

    bool ImGuiLink::BeginPopupContextItem()
    {
        return ImGui::BeginPopupContextItem();
    }

    glm::vec2 ImGuiLink::GetWindowSize()
    {
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();

        return glm::vec2(windowWidth, windowHeight);
    }

    glm::vec2 ImGuiLink::GetWindowPos()
    {
        float windowPosX = (float)ImGui::GetWindowPos().x;
        float windowPosY = (float)ImGui::GetWindowPos().y;

        return glm::vec2(windowPosX, windowPosY);
    }

    glm::vec2 ImGuiLink::GetMousePos()
    {
        return ImVec2ToVec2(ImGui::GetMousePos());
    }

    glm::vec2 ImGuiLink::GetCursorPos()
    {
        return ImVec2ToVec2(ImGui::GetCursorPos());
    }

    void ImGuiLink::Columns(int columnCount, const char* id, bool border)
    {
        ImGui::Columns(columnCount, id, border);
    }

    bool ImGuiLink::IsItemHovered()
    {
        return ImGui::IsItemHovered();
    }

    bool ImGuiLink::IsMouseDoubleClicked(ImGuiMouseButton mouseButton)
    {
        return ImGui::IsMouseDoubleClicked(mouseButton);
    }

    void ImGuiLink::TextWrapped(std::string str)
    {
        ImGui::TextWrapped(str.c_str());

    }

    void ImGuiLink::NextColumn()
    {
        return ImGui::NextColumn();
    }

    bool ImGuiLink::BeginDragDropSource()
    {
        return ImGui::BeginDragDropSource();
    }

    bool ImGuiLink::SetDragDropPayload(const char* type, const void* data, size_t data_size)
    {
        return ImGui::SetDragDropPayload(type, data, data_size);
    }

    ImGuiLink::ImGuiPayload* ImGuiLink::AcceptDragDropPayload(const char* name)
    {
        return (ImGuiLink::ImGuiPayload*)ImGui::AcceptDragDropPayload(name);
    }

    bool ImGuiLink::BeginDragDropTarget()
    {
        return ImGui::BeginDragDropTarget();
    }

    void ImGuiLink::EndDragDropTarget()
    {
        ImGui::EndDragDropTarget();
    }

    void ImGuiLink::EndDragDropSource()
    {
        ImGui::EndDragDropSource();
    }

    


}

