#include "vxpch.h"
#include "AudioManager.h"

namespace Vertex
{
	struct AudioManagerData
	{
		std::map<const std::filesystem::path, Audio*> AudioDictionary;
	};

	static AudioManagerData* s_Data;

	AudioManager* AudioManager::s_main;

	bool AudioManager::Init()
	{
		if (s_main != nullptr) { return true; }
		s_Data = new AudioManagerData();
		bool v = true;

		s_main = new AudioManager();

		return v;
	}
	Audio* AudioManager::GetAudioFromFileName(const std::filesystem::path& fileName, AudioSpecification& specification)
	{
		Audio* audio = nullptr;

		if (s_main == nullptr)
		{
			AudioManager::Init();
		}

		if (s_Data->AudioDictionary[fileName] != nullptr)
		{
			audio = s_Data->AudioDictionary[fileName];
		}
		else
		{
			audio = Audio::Create(fileName, specification);
		}


			

		return audio;
	}

	void AudioManager::AddAudioFromObj(const std::filesystem::path& fileName, Audio* obj)
	{
		if (s_main == nullptr)
		{
			AudioManager::Init();
		}

		if (s_Data->AudioDictionary[fileName] != nullptr)
		{

			return;
		}
		s_Data->AudioDictionary[fileName] = obj;

	}

	void AudioManager::Bye()
	{
		if (s_main == nullptr) { return; }
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

