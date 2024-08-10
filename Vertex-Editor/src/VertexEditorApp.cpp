#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Vertex {

	class VertexEditor : public Application
	{
	public:
		VertexEditor()
			: Application("Vertex Editor")
		{
			PushLayer(new EditorLayer());
		}

		~VertexEditor()
		{
		}
	};

	
	Application* Vertex::CreateApp()
	{
		return new Vertex::VertexEditor();
	}
}

