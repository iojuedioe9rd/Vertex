#include <Vertex/Core/Base.h>
#include "Scene/Scene.h"
#include <string>

#define VXEntities_INIT_USE_ENTITIES BIT(0)
//#define VXEntities_INIT_USE_MONO	 BIT(1)

#define VXEntities_INIT_USE_EVERYTHING (VXEntities_INIT_USE_ENTITIES)

bool VXEntities_INIT(uint32_t flags);
bool VXEntities_FREE();

namespace Vertex {
	Scene* VXEntities_MakeOrGetScene(std::string name);
	bool VXEntities_RemoveScene(Scene* scene);
	
}