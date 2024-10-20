#include <vxpch.h>
#include "imgui.h"
#include "ImGuizmoLink.h"
#include "ImGuizmo.h"

namespace Vertex
{
	void ImGuizmoLink::SetOrthographic(bool isOrthographic)
	{
		ImGuizmo::SetOrthographic(isOrthographic);
	}

	void ImGuizmoLink::SetDrawlist()
	{
		ImGuizmo::SetDrawlist();
	}

	void ImGuizmoLink::SetRect(glm::vec4 rect)
	{
		ImGuizmo::SetRect(rect.x, rect.y, rect.z, rect.w);
	}

	void ImGuizmoLink::Manipulate(const float* view, const float* projection, OPERATION operation, MODE mode, float* matrix, float* deltaMatrix, float* snap, float* localBounds, float* boundsSnap)
	{
		ImGuizmo::Manipulate(view, projection, (ImGuizmo::OPERATION)operation, (ImGuizmo::MODE)mode, matrix, deltaMatrix, snap, localBounds, boundsSnap);
	}

	bool ImGuizmoLink::IsUsing()
	{
		return false;
	}
}