#pragma once
#include "Vertex/Core/PlatformDetection.h"
#include <random>


namespace Vertex
{
	class VERTEX_API Random
	{
	public:


		static void Init(SafeObject<int64_t>& seed = SafeObject<int64_t>())
		{
			if (!seed)
			{
				seed.object = time(NULL);
			}

			m_seed = seed.object;

			//s_RandomEngine.seed(std::random_device()());
		}

		static int64_t RandomInt64()
		{
			
			return (m_seed = (164603309694725029ull * m_seed) % 14738995463583502973ull);
		}

		static float Float()
		{
			return (float)RandomInt64() / 14738995463583502973.0f;
			//return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<std::mt19937::result_type>::max();
		}

	private:
		static int64_t m_seed;
		//static std::mt19937 s_RandomEngine;
		//static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}