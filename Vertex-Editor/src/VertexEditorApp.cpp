#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor(ApplicationSettings& settings)
			: Application(settings)
		{
			
			PushLayer(new EditorLayer());
		}

		~VertexEditor()
		{
			
		}
	};

	
	Application* Vertex::CreateApp(ApplicationCommandLineArgs args)
	{
		ApplicationSettings settings;
		// "Vertex Editor", 1600, 900, args
		settings.Name = "Vertex Editor";
		settings.Width = 1600;
		settings.Height = 900;
		settings.Args = args;
		return new Vertex::VertexEditor(settings);
	}
}

