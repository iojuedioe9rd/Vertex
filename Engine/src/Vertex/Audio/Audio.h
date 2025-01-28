#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/AssetManager/Asset.h"
#include <string>


namespace Vertex
{

	class VERTEX_API Audio : public Asset
	{
	public:
		
		

		virtual void Play() = 0;
		virtual void Stop() = 0;

		static Audio* Create(std::filesystem::path filepath, bool loop = 0);

		virtual AssetType GetType() const override { return AssetType::Audio; }
	};

	

}