#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor(ApplicationCommandLineArgs args)
			: Application("Vertex Editor", 1600, 900, args)
		{
			
			PushLayer(new EditorLayer());
		}

		~VertexEditor()
		{
			
		}
	};

	
	Application* Vertex::CreateApp(ApplicationCommandLineArgs args)
	{
		return new Vertex::VertexEditor(args);
	}
}

