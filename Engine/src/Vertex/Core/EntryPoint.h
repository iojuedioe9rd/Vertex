#pragma once
#include "Vertex/Core/PlatformDetection.h"
#include "Vertex/Core/Application.h"
#include "Vertex/Core/Logger.h"
#include "Vertex/Debug/Instrumentor.h"
#include "Vertex/Audio/AudioManager.h"
#ifdef VX_PLATFORM_WINDOWS

#include "Platform/Windows/WindowsEntryPoint.h"

extern int Vertex_main(int argc, char** argv)
{

	VX_PROFILE_BEGIN_SESSION("Startup", "VertexProfile-Startup.json");
	Vertex::Logger::Init();
	Vertex::AudioManager::Init();
	Vertex::Application* app = Vertex::CreateApp({ argc, argv });
	VX_PROFILE_END_SESSION();
	VX_PROFILE_BEGIN_SESSION("Runtime", "VertexProfile-Runtime.json");
	app->Run();
	VX_PROFILE_END_SESSION();

	Vertex::AudioManager::Bye();
	delete app;

	return 0;
}

int console_ansi_main(int argc, char* argv[]) {
	return Vertex_main_getcmdline(Vertex_main);
}

#if UNICODE
int console_wmain(int argc, wchar_t* wargv[], wchar_t* wenvp) {
	return Vertex_main_getcmdline(Vertex_main);
}
#endif

#ifndef VERTEX_DLL
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
	return Vertex_main_getcmdline(Vertex_main);
}
#endif // !VERTEX_DLL

#endif