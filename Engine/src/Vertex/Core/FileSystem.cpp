#include "vxpch.h"
#include "FileSystem.h"

namespace Vertex
{
	Buffer* FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		Buffer* buffer = new Buffer(size);
		stream.read(buffer->As<char>(), size);
		stream.close();
		return buffer;
	}
}