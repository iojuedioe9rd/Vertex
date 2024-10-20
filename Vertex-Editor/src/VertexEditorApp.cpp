#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
#include "VXEntities.h"
#include "EditorLayer.h"

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor(ApplicationCommandLineArgs args)
			: Application("Vertex Editor", 1600, 900, args)
		{
			VXEntities_INIT(VXEntities_INIT_USE_EVERYTHING);
			PushLayer(new EditorLayer());
		}

		~VertexEditor()
		{
			VXEntities_FREE();
		}
	};

	
	Application* Vertex::CreateApp(ApplicationCommandLineArgs args)
	{
		return new Vertex::VertexEditor(args);
	}
}

