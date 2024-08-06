#pragma once
#include "Base.h"

#define MillisecondsToSeconds(t) (t / 1000.0f)
#define SecondsToMilliseconds(t) (t * 1000.0f)

namespace Vertex {

	class VERTEX_API Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return SecondsToMilliseconds(m_Time); }
	private:
		float m_Time;
	};

}