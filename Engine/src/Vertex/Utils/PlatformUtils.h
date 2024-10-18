#pragma once

#include "Vertex/Core/Base.h"

#include <string>
// 568

namespace Vertex {

	class VERTEX_API FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}