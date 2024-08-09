#pragma once
#include "Vertex/Audio/Audio.h"
#include "miniaudio.h"

namespace Vertex
{
	class WindowsAudio : public Audio
	{
	public:
		WindowsAudio(const std::string& filepath, bool loop);
		~WindowsAudio();

		virtual void Play() override;
		virtual void Stop() override;

	private:
		const std::string& m_filepath;
		bool m_loop;

		ma_sound m_sound;
	};

}