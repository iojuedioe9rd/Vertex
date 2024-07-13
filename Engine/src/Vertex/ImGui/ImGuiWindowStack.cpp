#include "vxpch.h"
#include "ImGuiWindowStack.h"

namespace Vertex::ImGuiWindows
{
	ImGuiWindowStack::~ImGuiWindowStack()
	{
		for (BaseImGuiWindow* imGuiWindow : m_ImGuiWindows)
		{
			imGuiWindow->OnDetach();
			delete imGuiWindow;
		}
	}
	void ImGuiWindowStack::PushImGuiWindow(BaseImGuiWindow* imGuiWindow)
	{
		m_ImGuiWindows.emplace(m_ImGuiWindows.begin() + m_ImGuiWindowsInsertIndex, imGuiWindow);
		m_ImGuiWindowsInsertIndex++;
	}
	void ImGuiWindowStack::PushImGuiWindowOverlay(BaseImGuiWindow* overlayImGuiWindow)
	{
		m_ImGuiWindows.emplace_back(overlayImGuiWindow);
	}
	void ImGuiWindowStack::PopImGuiWindow(BaseImGuiWindow* imGuiWindow)
	{
		auto it = std::find(m_ImGuiWindows.begin(), m_ImGuiWindows.begin() + m_ImGuiWindowsInsertIndex, imGuiWindow);
		if (it != m_ImGuiWindows.begin() + m_ImGuiWindowsInsertIndex)
		{
			imGuiWindow->OnDetach();
			m_ImGuiWindows.erase(it);
			m_ImGuiWindowsInsertIndex--;
		}
	}
	void ImGuiWindowStack::PopImGuiWindowOverlay(BaseImGuiWindow* overlayImGuiWindow)
	{
		auto it = std::find(m_ImGuiWindows.begin() + m_ImGuiWindowsInsertIndex, m_ImGuiWindows.end(), overlayImGuiWindow);
		if (it != m_ImGuiWindows.end())
		{
			overlayImGuiWindow->OnDetach();
			m_ImGuiWindows.erase(it);
		}
	}
}