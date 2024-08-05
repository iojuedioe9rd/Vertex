#include "Checksum.h"

namespace Vertex
{
	VERTEX_API_CHECKSUM uint32_t customChecksum(const std::vector<uint8_t>& data, uint32_t initial_value)
	{
        uint32_t checksum = initial_value;

        for (uint8_t byte : data) {
            // Add byte value to checksum
            checksum += byte;

            // Rotate the checksum left by 5 bits and XOR with original checksum
            uint32_t rotated = (checksum << 5) | (checksum >> (32 - 5));
            checksum ^= rotated;

            // Apply a modular operation to keep checksum within a 32-bit range
            checksum &= 0xFFFFFFFF;
        }

        return checksum;
	}
}