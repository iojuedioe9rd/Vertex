#include "vxpch.h"
#include "AudioManager.h"

namespace Vertex
{
	struct AudioManagerData
	{
		std::map<std::string, Audio*> AudioDictionary;
	};

	static AudioManagerData* s_Data;

	AudioManager* AudioManager::s_main;

	bool AudioManager::Init()
	{
		s_Data = new AudioManagerData();
		bool v = true;

		s_main = new AudioManager();

		return v;
	}
	Audio* AudioManager::GetAudioFromFileName(std::string& fileName, bool looping)
	{
		Audio* audio = nullptr;

		if (s_Data->AudioDictionary[fileName] != nullptr)
		{
			audio = s_Data->AudioDictionary[fileName];
		}
		else
		{
			audio = Audio::Create(fileName, looping);
		}


			

		return audio;
	}

	void AudioManager::AddAudioFromObj(std::string& fileName, Audio* obj)
	{
		if (s_Data->AudioDictionary[fileName] != nullptr)
		{

			return;
		}
		s_Data->AudioDictionary[fileName] = obj;

	}

	void AudioManager::Bye()
	{
		delete s_main;
	}

	AudioManager::AudioManager()
	{
	}

	AudioManager::~AudioManager()
	{
		for (const auto& pair : s_Data->AudioDictionary) {
			delete pair.second;
		}
		s_Data->AudioDictionary.clear();
		
		delete s_Data;
	}

}

