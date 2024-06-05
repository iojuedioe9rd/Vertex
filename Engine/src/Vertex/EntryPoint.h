#pragma once
#include "Vertex/PlatformDetection.h"
#include "Vertex/Application.h"
#ifdef VX_PLATFORM_WINDOWS

extern Vertex::Application* Vertex::CreateApp();

int main(int argc, char** argv)
{
	Vertex::Application* app = Vertex::CreateApp();
	app->Run();
	delete app;
}

#endif