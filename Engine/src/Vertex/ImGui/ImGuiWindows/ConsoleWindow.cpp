#include "vxpch.h"
#include "ConsoleWindow.h"
#include <list>
#include <algorithm>
#include "imgui.h"
#include "Vertex/ImGui/ImGuiLink.h"

namespace Vertex::ImGuiWindows
{
	std::list<ConsoleData*> l;

	bool show_core_log = 0;
	
	ConsoleWindow* ConsoleWindow::thisObj;
	void ConsoleWindow::OnUpdate()
	{
		ImGuiLink::Begin("Console", (bool*)0, ImGuiLink::ImGuiWindowFlags_MenuBar);
		if (ImGuiLink::BeginMenuBar())
		{
			if (ImGuiLink::BeginMenu("Settings"))
			{
				if (ImGuiLink::MenuItem("Core Log")) { show_core_log = !show_core_log; }
				
				ImGuiLink::EndMenu();
			}
			ImGuiLink::EndMenuBar();
		}
		for (std::list<ConsoleData*>::iterator i = l.begin(); i != l.end(); ++i)
		{
			ConsoleData* data = (*i);

			
			if (data->isCoreLogger && (!show_core_log))
			{
				
			}
			else
			{
				ImGuiLink::Text((*i)->msg.c_str());
			}
			
		}
		

		ImGui::End();
	}

	void ConsoleWindow::UpdateLogMsg(ConsoleData* msg)
	{
		l.push_back(msg);
	}
	ConsoleWindow::~ConsoleWindow()
	{
		ConsoleWindow::thisObj = NULL;
		for (std::list<ConsoleData*>::iterator i = l.begin(); i != l.end(); ++i)
			delete (*i);
	}
}