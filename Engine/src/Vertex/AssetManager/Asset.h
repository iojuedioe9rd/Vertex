#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/Object/Object.h"

namespace Vertex
{
	using AssetHandle = IntUUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	class VERTEX_API Asset: public Object
	{
	public:
		AssetHandle Handle; // Generate handle

		virtual AssetType GetType() const = 0;
	};

	
}