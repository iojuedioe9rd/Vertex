#include "vxpch.h"
#include "Audio.h"
#ifdef VX_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsAudio.h"
#endif // DEBUG



namespace Vertex
{

	Audio* Audio::Create(const char* filepath, bool loop)
	{
#ifdef VX_PLATFORM_WINDOWS
		return new WindowsAudio(filepath, loop);
#endif
	}

}