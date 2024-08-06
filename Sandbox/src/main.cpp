#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
//#include "ExampleLayer.h"
#include "Sandbox2D.h"


class Sandbox : public Vertex::Application
{
public:
	Sandbox()
	{
		VX_CORE_TRACE("Hi");
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}

private:

};


Vertex::Application* Vertex::CreateApp()
{
	return new Sandbox();
}