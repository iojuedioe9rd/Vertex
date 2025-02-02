#include <vxpch.h>
#include "Random.h"

namespace Vertex
{
	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
}