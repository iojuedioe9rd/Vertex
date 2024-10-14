#include "vxpch.h"
#include "WindowsAudio.h"
#include <Windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <filesystem>

#include "miniaudio.h"

#pragma once(lib, Winmm.lib)

ma_engine engine;

namespace Vertex {

	uint16_t AudioRefCount = 0;

	void LoadAudioFromResource(ma_engine* engine, int resourceId, ma_sound* sound)
	{
		HMODULE hModule = GetModuleHandle(NULL);
		HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(resourceId), RT_RCDATA);

		if (hRes == NULL) {
			std::cerr << "Resource not found for ID: " << resourceId << std::endl;
			return; // Resource not found
		}

		HGLOBAL hMem = LoadResource(hModule, hRes);
		if (hMem == NULL) {
			std::cerr << "Could not load resource for ID: " << resourceId << std::endl;
			return; // Could not load resource
		}

		void* pData = LockResource(hMem);
		DWORD dataSize = SizeofResource(hModule, hRes);

		// Write the data to a temporary file
		const char* tempFilePath = "temp.wav"; // Define a temporary file path
		FILE* tempFile = fopen(tempFilePath, "wb");
		if (tempFile != NULL) {
			fwrite(pData, 1, dataSize, tempFile);
			fclose(tempFile);

			// Now initialize the sound from the temporary file
			ma_result result = ma_sound_init_from_file(engine, tempFilePath, 0, NULL, NULL, sound);
			
			if (result != MA_SUCCESS) {
				std::cerr << "Error initializing sound from file ID: " << resourceId << " Error code: " << result << std::endl;
			}
			else {
				std::cout << "Loaded audio from resource ID: " << resourceId << std::endl;
			}

			// Clean up the temporary file
			remove(tempFilePath);
		}
		else {
			std::cerr << "Could not create temporary file." << std::endl;
		}

		// Clean up the resource
		FreeResource(hRes); // Properly release the resource
	}
	WindowsAudio::WindowsAudio(const char* filepath, bool loop)
		: m_filepath(filepath), m_loop(loop)
	{
		// Initialize audio engine on first audio instance
		if (AudioRefCount == 0)
		{
			ma_result result = ma_engine_init(NULL, &engine);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ENGINE ERROR!");
			}
		}

		++AudioRefCount;

		// Initialize sound
		ma_result result = ma_sound_init_from_file(&engine, m_filepath, 0, NULL, NULL, &m_sound);
		if (result != MA_SUCCESS) {
			VX_CORE_ASSERT(0, "AUDIO ERROR!");
		}

		ma_sound_set_looping(&m_sound, loop);
	}

	WindowsAudio::WindowsAudio(int resourceId, bool loop)
		: m_loop(loop)
	{
		// Initialize audio engine on first audio instance
		if (AudioRefCount == 0)
		{
			ma_result result = ma_engine_init(NULL, &engine);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ENGINE ERROR!");
			}
		}

		++AudioRefCount;
		LoadAudioFromResource(&engine, resourceId, &m_sound);
		ma_sound_set_looping(&m_sound, loop);
	}

	WindowsAudio::~WindowsAudio()
	{
		// Uninitialize the sound
		ma_sound_uninit(&m_sound);

		--AudioRefCount;

		// Uninitialize engine if this was the last audio instance
		if (AudioRefCount == 0)
		{
			ma_engine_uninit(&engine);
		}
	}

	void WindowsAudio::Play()
	{
		VX_CORE_INFO("Playing Audio: {0}", m_filepath);
		ma_sound_start(&m_sound);
	}

	void WindowsAudio::Stop()
	{
		VX_CORE_INFO("Stoping Audio: {0}", m_filepath);
		ma_sound_stop(&m_sound);
	}

	void WindowsAudio::SetVolume(float volume)
	{
		VX_CORE_INFO("Setting Volume: {0} for Audio: {1}", volume, m_filepath);
		ma_sound_set_volume(&m_sound, volume);
	}
}
