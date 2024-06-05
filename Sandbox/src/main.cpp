#include <Vertex/EntryPoint.h>
#include <Vertex.h>

class Sandbox : public Vertex::Application
{
public:
	Sandbox()
	{
		
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