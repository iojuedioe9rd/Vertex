#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Vertex {

	class GameApp : public Application
	{
	public:
		GameApp()
			: Application("Game")
		{
			PushLayer(new EditorLayer());
			
		}

		~GameApp()
		{
		}
	};

	
	Application* Vertex::CreateApp()
	{
		return new Vertex::GameApp();
	}
}

