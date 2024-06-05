#pragma once
#include "Vertex/Core.h"

namespace Vertex
{

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	extern Vertex::Application* CreateApp();
	

}
