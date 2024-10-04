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

	WindowsAudio::WindowsAudio(const char* filepath, bool loop)
		: m_filepath(filepath), m_loop(loop)
	{
		
		if (AudioRefCount == 0)
		{
			ma_result result = ma_engine_init(NULL, &engine);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ENGINE ERORR!");
			}

			

			
		}
		++AudioRefCount;
		{
			ma_result result;

			result = ma_sound_init_from_file(&engine, m_filepath, 0, NULL, NULL, &m_sound);

			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ERORR!");
			}
			ma_sound_set_looping(&m_sound, loop);
		}

	}

	WindowsAudio::~WindowsAudio()
	{
		--AudioRefCount;

		if (AudioRefCount == 0)
		{
			ma_sound_uninit(&m_sound);
		}

		
	}

	void WindowsAudio::Play()
	{
		
		VX_CORE_INFO("Playing Audio: {0}", m_filepath);

		

		ma_sound_start(&m_sound);
	}

	void WindowsAudio::Stop()
	{
	}

}