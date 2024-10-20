#pragma once
#include <Vertex/Core/Base.h>
#include <glm/glm.hpp>


namespace Vertex
{
	class VERTEX_API ImGuizmoLink
	{
	
	public:
        enum OPERATION: uint32_t
        {
            TRANSLATE_X = BIT(0),
            TRANSLATE_Y = BIT(1),
            TRANSLATE_Z = BIT(2),
            ROTATE_X = BIT(3),
            ROTATE_Y = BIT(4),
            ROTATE_Z = BIT(5),
            ROTATE_SCREEN = BIT(6),
            SCALE_X = BIT(7),
            SCALE_Y = BIT(8),
            SCALE_Z = BIT(9),
            BOUNDS = BIT(10),
            SCALE_XU = BIT(11),
            SCALE_YU = BIT(12),
            SCALE_ZU = BIT(13),

            TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
            ROTATE = ROTATE_X | ROTATE_Y | ROTATE_Z | ROTATE_SCREEN,
            SCALE = SCALE_X | SCALE_Y | SCALE_Z,
            SCALEU = SCALE_XU | SCALE_YU | SCALE_ZU, // universal
            UNIVERSAL = TRANSLATE | ROTATE | SCALEU
        };

        enum COLOR
        {
            DIRECTION_X,      // directionColor[0]
            DIRECTION_Y,      // directionColor[1]
            DIRECTION_Z,      // directionColor[2]
            PLANE_X,          // planeColor[0]
            PLANE_Y,          // planeColor[1]
            PLANE_Z,          // planeColor[2]
            SELECTION,        // selectionColor
            INACTIVE,         // inactiveColor
            TRANSLATION_LINE, // translationLineColor
            SCALE_LINE,
            ROTATION_USING_BORDER,
            ROTATION_USING_FILL,
            HATCHED_AXIS_LINES,
            TEXT,
            TEXT_SHADOW,
            COUNT
        };


        

        enum MODE
        {
            LOCAL,
            WORLD
        };

        static void SetOrthographic(bool isOrthographic);
        static void SetDrawlist();
        static void SetRect(glm::vec4 rect);
        static void Manipulate(const float* view, const float* projection, OPERATION operation, MODE mode, float* matrix, float* deltaMatrix = NULL, float* snap = NULL, float* localBounds = NULL, float* boundsSnap = NULL);
        static bool IsUsing();


	private:

	};

	
}


inline Vertex::ImGuizmoLink::OPERATION operator|(Vertex::ImGuizmoLink::OPERATION lhs, Vertex::ImGuizmoLink::OPERATION rhs)
{
    return static_cast<Vertex::ImGuizmoLink::OPERATION>(static_cast<int>(lhs) | static_cast<int>(rhs));
}