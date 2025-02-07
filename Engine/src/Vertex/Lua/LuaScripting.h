#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/AssetManager/Asset.h"

namespace Vertex
{
	

	class VERTEX_API LuaScripting
	{
	public:
		static void Init();
		static void Shutdown();

		static void RunTests();

	private:

	};

	
}