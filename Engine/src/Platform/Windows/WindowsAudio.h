#pragma once
#include "Vertex/Audio/Audio.h"
#include "miniaudio.h"

namespace Vertex
{
	class WindowsAudio : public Audio
	{
	public:
		WindowsAudio(std::filesystem::path filepath, bool loop);

		~WindowsAudio();

		virtual void Play() override;
		virtual void Stop() override;

	private:
		std::filesystem::path m_filepath;
		bool m_loop;

		ma_sound m_sound;
		static uint16_t s_RefCount;
	};

}