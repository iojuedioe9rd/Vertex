#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
#include "VXEntities.h"
#include "EditorLayer.h"

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor()
			: Application("Vertex Editor")
		{
			VXEntities_INIT(VXEntities_INIT_USE_EVERYTHING);
			PushLayer(new EditorLayer());
		}

		~VertexEditor()
		{

			VXEntities_FREE();
		}
	};

	
	Application* Vertex::CreateApp()
	{
		return new Vertex::VertexEditor();
	}
}

