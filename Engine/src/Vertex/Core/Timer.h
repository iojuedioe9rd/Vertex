#pragma once

#include <chrono>
#include <Vertex/Core/Base.h>

namespace Vertex {

	class VERTEX_API Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed() const
		{
			return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_Start).count();
		}

		float ElapsedMillis() const
		{
			return Elapsed() * 1000.0f;
		}

		float ElapsedSeconds() const
		{
			return Elapsed(); // already in seconds
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}
