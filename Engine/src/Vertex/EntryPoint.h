#pragma once
#include "Vertex/PlatformDetection.h"
#include "Vertex/Application.h"
#include "Vertex/Logger.h"
#include "Vertex/Debug/Instrumentor.h"
#ifdef VX_PLATFORM_WINDOWS

extern Vertex::Application* Vertex::CreateApp();

int main(int argc, char** argv)
{
	VX_PROFILE_BEGIN_SESSION("Startup", "VertexProfile-Startup.json");
	Vertex::Logger::Init();
	Vertex::Application* app = Vertex::CreateApp();
	VX_PROFILE_END_SESSION();
	VX_PROFILE_BEGIN_SESSION("Runtime", "VertexProfile-Runtime.json");
	app->Run();
	VX_PROFILE_END_SESSION();
	delete app;
}

#endif