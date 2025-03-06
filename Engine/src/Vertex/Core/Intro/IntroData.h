#pragma once

#include <Vertex/Core/Base.h>
#include <Vertex/Renderer/Texture.h>

namespace Vertex
{
	void VERTEX_API SetupIntroData();

	Ref<Texture2D> VERTEX_API GetCatTexture();

	Ref<Texture2D> VERTEX_API GetCircleTexture();
}
