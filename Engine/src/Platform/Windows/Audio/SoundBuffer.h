#pragma once
#include "Vertex/Core/Base.h"

namespace Vertex
{
	class VERTEX_API WindowsSoundBuffer
	{
	public:
		static WindowsSoundBuffer* get();
		static void destroy();

		ALuint addSoundEffect(const std::fs::path filepath);
		bool removeSoundEffect(const ALuint& buffer);

	private:
		WindowsSoundBuffer();
		~WindowsSoundBuffer();

		std::vector<ALuint> m_SoundEffectsBuffers;

		static WindowsSoundBuffer* m_Instance;
	};
}