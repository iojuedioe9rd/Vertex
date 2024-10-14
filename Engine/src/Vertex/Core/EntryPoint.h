#pragma once
#include "Vertex/Core/PlatformDetection.h"
#include "Vertex/Core/Application.h"
#include "Vertex/Core/Logger.h"
#include "Vertex/Debug/Instrumentor.h"
#include "Vertex/Audio/AudioManager.h"
#ifdef VX_PLATFORM_WINDOWS

extern Vertex::Application* Vertex::CreateApp();

#include <windows.h>

int main()
{
	VX_PROFILE_BEGIN_SESSION("Startup", "VertexProfile-Startup.json");
	Vertex::Logger::Init();
	Vertex::AudioManager::Init();
	Vertex::Application* app = Vertex::CreateApp();
	VX_PROFILE_END_SESSION();
	VX_PROFILE_BEGIN_SESSION("Runtime", "VertexProfile-Runtime.json");
	app->Run();
	VX_PROFILE_END_SESSION();

	Vertex::AudioManager::Bye();
	delete app;
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{

	return main();
}


#endif