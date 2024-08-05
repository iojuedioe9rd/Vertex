#pragma once
#include <cstdint>
#include <vector>

namespace Vertex
{
#ifdef VX_BUILD_DLL_CHECKSUM
#define VERTEX_API_CHECKSUM __declspec(dllexport)
#else
#define VERTEX_API_CHECKSUM __declspec(dllimport)
#endif

	VERTEX_API_CHECKSUM uint32_t customChecksum(const std::vector<uint8_t>& data, uint32_t initial_value = 0);
}