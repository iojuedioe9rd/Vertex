#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/ImGui/BaseImGuiWindow.h"
using namespace Vertex;
namespace Vertex::ImGuiWindows
{
	
	struct VERTEX_API ConsoleData
	{
		bool isCoreLogger;
		std::string msg;
		

	};

	class VERTEX_API ConsoleWindow : public BaseImGuiWindow
	{
	public:
		

		void OnUpdate() override;

		void UpdateLogMsg(ConsoleData* data);

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