#include "MainLib.h"
#include "Vertex/Core/Logger.h"
#include "unordered_map"

std::unordered_map<std::string, Vertex::Scene*> Scenes;

bool useEntities = false;

 

bool VXEntities_INITEntities()
{
	useEntities = true;
	Scenes = std::unordered_map<std::string, Vertex::Scene*>();
	return true;
}

bool VXEntities_INIT(uint32_t flags)
{
	bool s = true;
	if (flags & VXEntities_INIT_USE_ENTITIES)
	{
		s = VXEntities_INITEntities();
		if (!s)
		{
			return s;
		}
	}

}

bool VXEntities_FREE()
{
	if (useEntities)
	{
		for (auto& pair : Scenes) {
			
			
		}
	}

	return 1;
	
}

namespace Vertex
{
	Scene* VXEntities_MakeOrGetScene(std::string name)
	{
		// Check if the scene already exists in the map
		auto it = Scenes.find(name);
		if (it != Scenes.end()) {
			return it->second; // Return the existing scene
		}

		// If not, create a new scene
		Scene* newScene = new Scene(name);
		Scenes[name] = newScene; // Add it to the map
		return newScene;
	}
}