#pragma once
#include "Vertex/Core/Base.h"
#include <glm/glm.hpp>
#include "../vendor/imgui/imgui_internal.h"

typedef int ImGuiStyleVar;

namespace Vertex
{
    

	class VERTEX_API ImGuiLink
	{


	public:

        struct VERTEX_API ImGuiPayload
        {
            // Members
            void* Data;               // Data (copied and owned by dear imgui)
            int             DataSize;           // Data size

            // [Internal]
            uint32_t        SourceId;           // Source item id
            uint32_t        SourceParentId;     // Source parent id (if available)
            int             DataFrameCount;     // Data timestamp
            char            DataType[32 + 1];     // Data type tag (short user-supplied string, 32 characters max)
            bool            Preview;            // Set when AcceptDragDropPayload() was called and mouse has been hovering the target item (nb: handle overlapping drag targets)
            bool            Delivery;           // Set when AcceptDragDropPayload() was called and mouse button is released over the target item.

            ImGuiPayload() { Clear(); }
            void Clear() { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
            bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
            bool IsPreview() const { return Preview; }
            bool IsDelivery() const { return Delivery; }
        };

        enum ImGuiWindowFlags: int
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

        enum ImGuiStyleVar_: int
        {
            // Enum name -------------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
            ImGuiStyleVar_Alpha,                    // float     Alpha
            ImGuiStyleVar_DisabledAlpha,            // float     DisabledAlpha
            ImGuiStyleVar_WindowPadding,            // ImVec2    WindowPadding
            ImGuiStyleVar_WindowRounding,           // float     WindowRounding
            ImGuiStyleVar_WindowBorderSize,         // float     WindowBorderSize
            ImGuiStyleVar_WindowMinSize,            // ImVec2    WindowMinSize
            ImGuiStyleVar_WindowTitleAlign,         // ImVec2    WindowTitleAlign
            ImGuiStyleVar_ChildRounding,            // float     ChildRounding
            ImGuiStyleVar_ChildBorderSize,          // float     ChildBorderSize
            ImGuiStyleVar_PopupRounding,            // float     PopupRounding
            ImGuiStyleVar_PopupBorderSize,          // float     PopupBorderSize
            ImGuiStyleVar_FramePadding,             // ImVec2    FramePadding
            ImGuiStyleVar_FrameRounding,            // float     FrameRounding
            ImGuiStyleVar_FrameBorderSize,          // float     FrameBorderSize
            ImGuiStyleVar_ItemSpacing,              // ImVec2    ItemSpacing
            ImGuiStyleVar_ItemInnerSpacing,         // ImVec2    ItemInnerSpacing
            ImGuiStyleVar_IndentSpacing,            // float     IndentSpacing
            ImGuiStyleVar_CellPadding,              // ImVec2    CellPadding
            ImGuiStyleVar_ScrollbarSize,            // float     ScrollbarSize
            ImGuiStyleVar_ScrollbarRounding,        // float     ScrollbarRounding
            ImGuiStyleVar_GrabMinSize,              // float     GrabMinSize
            ImGuiStyleVar_GrabRounding,             // float     GrabRounding
            ImGuiStyleVar_TabRounding,              // float     TabRounding
            ImGuiStyleVar_TabBorderSize,            // float     TabBorderSize
            ImGuiStyleVar_TabBarBorderSize,         // float     TabBarBorderSize
            ImGuiStyleVar_TableAngledHeadersAngle,  // float  TableAngledHeadersAngle
            ImGuiStyleVar_TableAngledHeadersTextAlign,// ImVec2 TableAngledHeadersTextAlign
            ImGuiStyleVar_ButtonTextAlign,          // ImVec2    ButtonTextAlign
            ImGuiStyleVar_SelectableTextAlign,      // ImVec2    SelectableTextAlign
            ImGuiStyleVar_SeparatorTextBorderSize,  // float  SeparatorTextBorderSize
            ImGuiStyleVar_SeparatorTextAlign,       // ImVec2    SeparatorTextAlign
            ImGuiStyleVar_SeparatorTextPadding,     // ImVec2    SeparatorTextPadding
            ImGuiStyleVar_COUNT
        };

        enum ImGuiMouseButton
        {
            Left,
            Right,
            Middle
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
        static bool ImageButton(void* imageID, glm::vec2 size, glm::vec2 uv0 = glm::vec2(0, 0), glm::vec2 uv1 = glm::vec2(1, 1));
        static void PushStyleVar(int styleVar, glm::vec2 idk);
        static void PopStyleVar();
        static glm::vec2 GetContentRegionAvail();
        static bool IsWindowFocused();
        static bool IsWindowHovered();
        static void Separator();
        static void DragFloat3(std::string label, float* value, float speed = 1.0f);
        static bool Checkbox(std::string label, bool* v);
        static bool TreeNodeEx(std::string srt_id, int Flags, std::string label);
        static bool TreeNodeEx(void* ptr_id, int Flags, std::string label);
        static bool IsItemClicked();
        static void TreePop();
        static bool InputText(char* sdd, char* buffer, size_t buffer_size);
        static bool IsMouseDown(int mouseButton);
        static bool BeginCombo(char* name, char* idk);
        static void EndCombo();
        static bool Selectable(char* name, bool* selected);
        static bool DragFloat(std::string label, float value_ptr[1], float speed = 0.0f, float min = 0.0f, float max = 0.0f);
        static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
        static bool BeginPopupContextWindow();
        static void EndPopup();
        static bool Button(std::string label, glm::vec2 size = glm::vec2(0,0));
        static bool BeginPopupContextItem();
        static glm::vec2 GetWindowSize();
        static glm::vec2 GetWindowPos();
        static glm::vec2 GetMousePos();
        static glm::vec2 GetCursorPos();
        static void Columns(int columnCount = 1, const char* id = 0, bool border = true);
        static bool IsItemHovered();
        static bool IsMouseDoubleClicked(ImGuiMouseButton mouseButton);
        static void TextWrapped(std::string str);
        static void NextColumn();

        static bool BeginDragDropSource();
        static bool SetDragDropPayload(const char* type, const void* data, size_t data_size);
        static ImGuiPayload* AcceptDragDropPayload(const char* name);
        static bool BeginDragDropTarget();
        static void EndDragDropTarget();
        static void EndDragDropSource();

        static void* GetContext();



	};



}

