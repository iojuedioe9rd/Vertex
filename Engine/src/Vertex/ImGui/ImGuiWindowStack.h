#pragma once

#include "Vertex/Core/Base.h"
#include <vector>
#include "BaseImGuiWindow.h"

namespace Vertex::ImGuiWindows
{
	class VERTEX_API ImGuiWindowStack
	{
	public:
		ImGuiWindowStack() = default;
		~ImGuiWindowStack();

		void PushImGuiWindow(BaseImGuiWindow* imGuiWindow);
		void PushImGuiWindowOverlay(BaseImGuiWindow* overlayImGuiWindow);
		void PopImGuiWindow(BaseImGuiWindow* imGuiWindow);
		void PopImGuiWindowOverlay(BaseImGuiWindow* overlayImGuiWindow);

		std::vector<BaseImGuiWindow*>::iterator begin() { return m_ImGuiWindows.begin(); }
		std::vector<BaseImGuiWindow*>::iterator end() { return m_ImGuiWindows.end(); }
		std::vector<BaseImGuiWindow*>::reverse_iterator rbegin() { return m_ImGuiWindows.rbegin(); }
		std::vector<BaseImGuiWindow*>::reverse_iterator rend() { return m_ImGuiWindows.rend(); }

		std::vector<BaseImGuiWindow*>::const_iterator begin() const { return m_ImGuiWindows.begin(); }
		std::vector<BaseImGuiWindow*>::const_iterator end()	const { return m_ImGuiWindows.end(); }
		std::vector<BaseImGuiWindow*>::const_reverse_iterator rbegin() const { return m_ImGuiWindows.rbegin(); }
		std::vector<BaseImGuiWindow*>::const_reverse_iterator rend() const { return m_ImGuiWindows.rend(); }


	private:
		std::vector<BaseImGuiWindow*> m_ImGuiWindows;
		unsigned int m_ImGuiWindowsInsertIndex = 0;
	};
}