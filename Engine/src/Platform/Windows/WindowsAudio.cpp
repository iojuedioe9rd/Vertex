#include "vxpch.h"
#include "WindowsAudio.h"
#include <Windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <filesystem>

#include "miniaudio.h"

#pragma once(lib, Winmm.lib)

bool isInit = 0;

uint16_t Vertex::WindowsAudio::s_RefCount = 0;


ma_engine engine;

namespace Vertex {



	WindowsAudio::WindowsAudio(std::filesystem::path filepath, bool loop)
		: m_filepath(filepath), m_loop(loop)
	{
		
		if (s_RefCount == 0)
		{
			ma_result result = ma_engine_init(NULL, &engine);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ENGINE ERORR!");
			}

			

			
		}
		s_RefCount++;
		{
			
			ma_result result;

			result = ma_sound_init_from_file(&engine, m_filepath.generic_string().c_str(), 0, NULL, NULL, &m_sound);

			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ERORR!");
			}
			ma_sound_set_looping(&m_sound, loop);
		}

	}

	

	WindowsAudio::~WindowsAudio()
	{
		s_RefCount--;
		ma_sound_uninit(&m_sound);

		if (s_RefCount == 0)
		{
			ma_engine_uninit(&engine);
		}
	}

	void WindowsAudio::Play()
	{
		
		VX_CORE_INFO("Playing Audio: {0}", m_filepath.generic_string().c_str());

		

		ma_sound_start(&m_sound);
	}

	void WindowsAudio::Stop()
	{
		ma_sound_stop(&m_sound);
	}

}