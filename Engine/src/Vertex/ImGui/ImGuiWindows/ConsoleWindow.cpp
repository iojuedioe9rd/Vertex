#include "vxpch.h"
#include "ConsoleWindow.h"
#include <list>
#include <algorithm>
#include "imgui.h"

namespace Vertex::ImGuiWindows
{
	std::list<std::string> l;

	
	ConsoleWindow* ConsoleWindow::thisObj;
	void ConsoleWindow::OnUpdate()
	{
		ImGui::Begin("Console");

		for (std::list<std::string>::iterator i = l.begin(); i != l.end(); ++i)
			ImGui::Text((*i).c_str());

		ImGui::End();
	}

	void ConsoleWindow::UpdateLogMsg(std::string msg)
	{
		l.push_back(msg);
	}
	ConsoleWindow::~ConsoleWindow()
	{
		ConsoleWindow::thisObj = NULL;
	}
}