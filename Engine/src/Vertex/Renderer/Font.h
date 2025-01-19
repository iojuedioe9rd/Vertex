#pragma once
#include "Vertex/Core/Base.h"

namespace msdfgen
{
	class FontHandle;
}

namespace Vertex
{
	class VERTEX_API Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

	private:
		msdfgen::FontHandle* m_FontHandle = nullptr;
	};
}