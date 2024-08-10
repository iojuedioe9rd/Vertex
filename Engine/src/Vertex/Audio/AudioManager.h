#pragma once

#include "Vertex/Core/Base.h"
#include "Audio.h"
#include <string>

namespace Vertex 
{
	class VERTEX_API AudioManager
	{
	public:
		
		static bool Init();

		static Audio* GetAudioFromFileName(const char* fileName, bool looping = 0);

		static void AddAudioFromObj(const char* fileName, Audio* obj);

		static void Bye();

		AudioManager();
		~AudioManager();

	private:

		static AudioManager* s_main;

		
	};

	
}