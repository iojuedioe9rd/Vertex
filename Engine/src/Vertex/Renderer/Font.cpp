#include "vxpch.h"
#include "Font.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "msdfgen.h"

#define ft			(::Vertex::Font_FreetypeHandle)
#define ref_count	(::Vertex::Font_FreetypeHandle_ref_count)

namespace Vertex
{
	uint16_t Font_FreetypeHandle_ref_count = 0;

	msdfgen::FreetypeHandle* Font_FreetypeHandle;


	Font::Font(const std::filesystem::path& font)
	{
		
		if (ref_count == 0)
		{
			ft = msdfgen::initializeFreetype();
		}
		ref_count++;

		if (!ft)
		{
			return;
		}
		std::string fileStr = font.string();
		m_FontHandle = msdfgen::loadFont(ft, fileStr.c_str());

		if (!m_FontHandle)
		{
			return;
		}

		msdfgen::Shape shape;
		if (msdfgen::loadGlyph(shape, m_FontHandle, 'A'))
		{
			shape.normalize();

			msdfgen::edgeColoringSimple(shape, 3);

			msdfgen::Bitmap<float, 4> msdf(64, 64);

			msdfgen::generateMTSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
			msdfgen::savePng(msdf, "output.png");
		}
	}

	Font::~Font()
	{
		msdfgen::destroyFont(m_FontHandle);

		ref_count--;
		if (ref_count == 0)
		{
			msdfgen::deinitializeFreetype(ft);
			ft = NULL;
		}
	}


}
