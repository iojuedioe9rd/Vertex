#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
//#include "ExampleLayer.h"
#include "Sandbox2D.h"


class Sandbox : public Vertex::Application
{
public:
	Sandbox(ApplicationCommandLineArgs args)
		: Application("Sandbox", 1600, 900, args)
	{
		VX_CORE_TRACE("Hi");
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}

private:

};


Vertex::Application* Vertex::CreateApp(ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}