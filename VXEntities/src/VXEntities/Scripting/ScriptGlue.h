#pragma once
#include "Vertex/AssetManager/AssetManagerBase.h"

namespace Vertex
{
	class ScriptGlue
	{
	public:
		static void RegisterComponents();
		static void RegisterFunctions();

		static void SetupGetingAssets(AssetHandle(*getAsset)(std::filesystem::path), AssetManagerBase*(*getAssetMan)());
	};
}