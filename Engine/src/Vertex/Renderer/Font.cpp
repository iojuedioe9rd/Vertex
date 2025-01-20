#include "vxpch.h"
#include "Font.h"
#include "MSDFData.h"

#undef INFINITE
#include "msdf-atlas-gen.h"
#include "FontGeometry.h"
#include "GlyphGeometry.h"

struct CharsetRange {
    uint32_t Begin, End;
};

namespace Vertex {

    // Constants for LCG, Threshold, and Thread Count
#define LCG_MULTIPLIER 6364136223846793005ull
#define LCG_INCREMENT 1442695040888963407ull
#define DEFAULT_ANGLE_THRESHOLD 3.0f  // Adjust threshold as needed
#define THREAD_COUNT 8  // Number of threads for parallel processing

    // Optimized texture creation with better memory management and clarity
    template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
    static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
        const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
    {
        msdf_atlas::GeneratorAttributes attributes;
        attributes.config.overlapSupport = true;
        attributes.scanlinePass = true;

        msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
        generator.setAttributes(attributes);
        generator.setThreadCount(8);
        generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

        msdfgen::BitmapConstRef<T, N> bitmap = generator.atlasStorage();

        TextureSpecification spec;
        spec.Width = bitmap.width;
        spec.Height = bitmap.height;
        spec.Format = ImageFormat::RGB8;
        spec.GenerateMips = true;  // Enable mipmaps for better scaling
        Ref<Texture2D> texture = Texture2D::Create(spec);
        texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * N);
        texture->ToFile("test.png");  // Optionally save the atlas as a file
        return texture;
    }

    // Constructor with improved memory handling and font loading
    Font::Font(const std::filesystem::path& filepath)
        : m_Data(new MSDFData())
    {
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
        VX_CORE_ASSERT(ft);

        std::string fileString = filepath.string();

        msdfgen::FontHandle* font = msdfgen::loadFont(ft, fileString.c_str());
        if (!font)
        {
            VX_CORE_ERROR("Failed to load font: {}", fileString);
            return;
        }

        // Dynamically load charset range
        static const std::vector<CharsetRange> charsetRanges = {
            { 0x0020, 0x00FF },  // Basic Latin + Latin-1 Supplement
            { 0x0100, 0x017F },  // Latin Extended-A (example)
            // Add other ranges if necessary
        };

        msdf_atlas::Charset charset;
        for (const CharsetRange& range : charsetRanges)
        {
            for (uint32_t c = range.Begin; c <= range.End; ++c)
                charset.add(c);
        }

        double fontScale = 1.0;
        m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
        int glyphsLoaded = m_Data->FontGeometry.loadCharset(font, fontScale, charset);
        VX_CORE_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

        // Adjust the font scaling dynamically or via config
        double emSize = 40.0;

        msdf_atlas::TightAtlasPacker atlasPacker;
        atlasPacker.setPixelRange(2.0);  // Adjust pixel range for higher quality
        atlasPacker.setMiterLimit(1.0);
        atlasPacker.setPadding(0);
        atlasPacker.setScale(emSize);
        int remaining = atlasPacker.pack(m_Data->Glyphs.data(), static_cast<int>(m_Data->Glyphs.size()));
        VX_CORE_ASSERT(remaining == 0);

        int width, height;
        atlasPacker.getDimensions(width, height);
        emSize = atlasPacker.getScale();

        // Simplified the coloring process and improved performance using the preferred method
        uint64_t coloringSeed = 0;
        bool expensiveColoring = true;

        if (expensiveColoring)
        {
            msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
                unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
                glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
                return true;
                }, m_Data->Glyphs.size()).finish(THREAD_COUNT);
        }
        else {
            unsigned long long glyphSeed = coloringSeed;
            for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
            {
                glyphSeed *= LCG_MULTIPLIER;
                glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
            }
        }

        // Create and cache atlas with improved parameters for better text rendering
        m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Test", static_cast<float>(emSize), m_Data->Glyphs, m_Data->FontGeometry, width, height);

        msdfgen::destroyFont(font);
        msdfgen::deinitializeFreetype(ft);
    }

    Ref<Font> Font::GetDefault()
    {
        static Ref<Font> DefaultFont;
        if (!DefaultFont)
            DefaultFont = CreateRef<Font>("assets/fonts/opensans/OpenSans-Regular.ttf");
        return DefaultFont;
    }

    Font::~Font()
    {
        delete m_Data;
    }

}
