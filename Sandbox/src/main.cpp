#include <Vertex/EntryPoint.h>
#include <Vertex.h>
#include "ExampleLayer.h"


class Sandbox : public Vertex::Application
{
public:
	Sandbox()
	{
		VX_CORE_TRACE("Hi");
		PushLayer(new ExampleLayer());
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