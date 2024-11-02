#pragma once

#include "Vertex/Core/Buffer.h"

namespace Vertex {

	class VERTEX_API FileSystem
	{
	public:
		static Buffer* ReadFileBinary(const std::filesystem::path& filepath);
	};
}