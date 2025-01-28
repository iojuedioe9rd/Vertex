#pragma once

#include <Vertex/Core/Base.h>
#include "Asset.h"
#include "AssetManagerBase.h"

namespace Vertex
{
    // Always declare the variable as extern
    inline Ref<AssetManagerBase> g_AssetManagerBase = Ref<AssetManagerBase>();
}
