#pragma once

#include "Vertex/Core/Base.h"

#include <vector>

#undef INFINITE
#include "msdf-atlas-gen.h"

namespace Vertex {

	struct VERTEX_API MSDFData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};
}