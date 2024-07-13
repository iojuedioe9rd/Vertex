#pragma once
#include "Vertex/PlatformDetection.h"
#include "Vertex/Core.h"
#include "Vertex/Logger.h"
#include "Vertex/Debug/Instrumentor.h"
#include "Vertex/CommandBuffer/CommandBuffer.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#ifdef VX_PLATFORM_WINDOWS

#include <Windows.h>

#endif // VX_PLATFORM_WINDOWS

