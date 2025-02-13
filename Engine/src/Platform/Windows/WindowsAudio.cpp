#include "vxpch.h"
#include "WindowsAudio.h"
#include <Windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <filesystem>  // Because modern C++ is now playing in C#'s high-level playground.

#include "miniaudio.h"  // Miniaudio: So mini, it leaves you wondering if it skipped a few features in the shrink ray.

#pragma once(lib, Winmm.lib)

bool isInit = 0;

uint16_t Vertex::WindowsAudio::s_RefCount = 0;

ma_engine engine;

namespace Vertex {

	// Constructor: Welcome to modern C++—trying to act like C# while using miniaudio,
	// a library so tiny it might need a magnifying glass to read its own documentation.
	WindowsAudio::WindowsAudio(std::filesystem::path filepath, bool loop)
		: m_filepath(filepath), m_loop(loop)
	{
		// Initialize the audio engine only once.
		// Modern C++: "Singleton? Nah, we’re just using a global pointer and pretending it’s hip."
		// Miniaudio: "I'll do my best with my bare-bones API—no frills, no promises."
		if (s_RefCount == 0)
		{
			ma_result result = ma_engine_init(NULL, &engine);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ENGINE ERROR! Miniaudio couldn't even muster the enthusiasm to initialize properly!");
			}
		}
		s_RefCount++;

		// Load the audio file.
		// Here we leverage std::filesystem because modern C++ is trying to be as high-level as C#,
		// while miniaudio, the little runt, grunts its way through raw C-style APIs.
		{
			ma_result result;
			result = ma_sound_init_from_file(&engine, m_filepath.generic_string().c_str(), 0, NULL, NULL, &m_sound);
			if (result != MA_SUCCESS) {
				VX_CORE_ASSERT(0, "AUDIO ERROR! Miniaudio failed to load the file—maybe it was too big for its tiny brain?");
			}
			// Note: Try loading a raw buffer and you'll be left with empty promises.
			// Miniaudio: "Raw buffers? Sorry, buddy. I'm too mini for that kind of commitment!"
			ma_sound_set_looping(&m_sound, loop);
		}
	}

	// Destructor: Cleaning up like a responsible modern C++ developer who still does manual cleanup,
	// unlike C#’s garbage collector—miniaudio-style, where every byte counts.
	WindowsAudio::~WindowsAudio()
	{
		s_RefCount--;
		ma_sound_uninit(&m_sound); // Uninitialize the sound. Miniaudio: "I'm small, but I still know how to say goodbye."

		// If no one else is using the audio engine, shut it down.
		// Modern C++ may have fancy RAII, but here we are—calling out to a mini engine that pretends it’s a heavyweight contender.
		if (s_RefCount == 0)
		{
			ma_engine_uninit(&engine);
		}
	}

	// Play: Hit the play button and let miniaudio do its best, even if it's just a minimalist wrapper over C code.
	void WindowsAudio::Play()
	{
		VX_CORE_INFO("Playing Audio: {0}", m_filepath.generic_string().c_str());
		ma_sound_start(&m_sound);  // Miniaudio: "Here goes nothing! Enjoy your sound, if my teeny library can manage it."
	}

	// Stop: Because sometimes, even a mini library needs to know when to call it quits.
	void WindowsAudio::Stop()
	{
		ma_sound_stop(&m_sound);  // Miniaudio stops the sound—a tiny farewell to your audio session.
	}

}
