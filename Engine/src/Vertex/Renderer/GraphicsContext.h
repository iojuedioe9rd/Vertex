#pragma once
#include "Vertex/Core.h"
namespace Vertex {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}