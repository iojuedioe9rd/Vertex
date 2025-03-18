#include <Vertex.h>
#include <Vertex/Core/EntryPoint.h>
//#include "ExampleLayer.h"
#include "Sandbox2D.h"


class Sandbox : public Vertex::Application
{
public:
	Sandbox(ApplicationSettings settings)
		: Application(settings)
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
	// Application("Sandbox", 1600, 900, args)
	ApplicationSettings settings;
	settings.Name = "Sandbox";
	settings.Width = 1600;
	settings.Height = 900;
	settings.Args = args;
	return new Sandbox(settings);
}