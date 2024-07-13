#pragma once
#include "Vertex/Core.h"
#include "Vertex/ImGui/BaseImGuiWindow.h"
using namespace Vertex;
namespace Vertex::ImGuiWindows
{
	
	class VERTEX_API ConsoleWindow : public BaseImGuiWindow
	{
	public:
		

		void OnUpdate() override;

		void UpdateLogMsg(std::string msg);

		static ConsoleWindow& Get()
		{
			if (thisObj == nullptr)
			{
				thisObj = new ConsoleWindow();
			}
			return *thisObj;
		}

		friend class Application;

	private:
		ConsoleWindow() {
			m_DebugName = "Console";
		};
		~ConsoleWindow();

		static ConsoleWindow* thisObj;
	};
}