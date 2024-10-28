#include <Vertex/Core/Base.h>
#include "Scene/Scene.h"
#include <string>

#define VXEntities_INIT_USE_ENTITIES BIT(0)
#define VXEntities_INIT_USE_SCENE_SERIALIZER BIT(1)
#define VXEntities_INIT_USE_MONO			 BIT(2)

#define VXEntities_INIT_USE_EVERYTHING (VXEntities_INIT_USE_ENTITIES | VXEntities_INIT_USE_SCENE_SERIALIZER | VXEntities_INIT_USE_MONO)

bool VXEntities_INIT(uint32_t flags);
uint32_t VXEntities_GET_FLAGS();
bool VXEntities_FREE();

namespace Vertex {
	Scene* VXEntities_MakeOrGetScene(std::string name);
	bool VXEntities_RemoveScene(Scene* scene);
	bool VXEntities_RemoveScene(std::string name);
}