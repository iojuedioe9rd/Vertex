#include "vxpch.h"
#include "Audio.h"
#ifdef VX_PLATFORM_WINDOWS
#include "Platform/Windows/Audio/WindowsAudio.h"
#endif // DEBUG



namespace Vertex
{

	Audio* Audio::Create(std::filesystem::path filepath, AudioSpecification& specification)
	{
#ifdef VX_PLATFORM_WINDOWS
		return new WindowsAudio(filepath, specification);
#endif
	}

}