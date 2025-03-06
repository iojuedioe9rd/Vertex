using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public static class ImGuiLink
    {
        public enum ImGuiWindowFlags : int
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

        public enum ImGuiStyleVar_ : int
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

        public static bool Begin(string name, bool open, ImGuiWindowFlags flags = ImGuiWindowFlags.ImGuiWindowFlags_None)
        {
            return InternalCalls.ImGui_Begin(name, ref open);
        }

        public static void End()
        {
            InternalCalls.ImGui_End();
        }

        public static bool ColourEdit(string name, ref Vector4 colour)
        {
            
            throw new NotImplementedException();
            return true;
        }

        public static void Text(params string[] fmt)
        {
            InternalCalls.ImGui_Text(fmt);
        }

    }
}
