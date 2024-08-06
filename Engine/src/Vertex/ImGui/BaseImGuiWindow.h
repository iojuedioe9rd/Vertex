#pragma once
#include "Vertex/Core/Base.h"

namespace Vertex::ImGuiWindows
{
	class VERTEX_API BaseImGuiWindow
	{
	public:
		BaseImGuiWindow(const std::string& name = "InGui Window");
		virtual ~BaseImGuiWindow() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;

	};
}