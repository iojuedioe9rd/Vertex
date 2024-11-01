#include "MainLib.h"
#include "Vertex/Core/Logger.h"
#include "unordered_map"
#include "Scripting/ScriptEngine.h"

std::unordered_map<std::string, Vertex::Scene*>* Scenes;

bool useEntities = false;

uint32_t flagsToUse = 0;

bool VXEntities_INITEntities()
{
	useEntities = true;
	Scenes = new std::unordered_map<std::string, Vertex::Scene*>();
	return true;
}

bool VXEntities_INIT_Scene_Serializer()
{
	bool s = true;
	VX_CORE_ASSERT(useEntities, "Entities not init!");
	s = useEntities;
	return s;
}

bool VXEntities_INIT_Mono()
{
	bool s = true;

	VX_CORE_ASSERT(useEntities, "Entities not init!");
	Vertex::ScriptEngine::Init();
	s = useEntities;
	return s;
}

bool VXEntities_INIT(uint32_t flags)
{
	flagsToUse |= flags;
	bool s = true;
	if (flags & VXEntities_INIT_USE_ENTITIES)
	{
		s = VXEntities_INITEntities();
		if (!s)
		{
			return s;
		}
	}
	if (flags & VXEntities_INIT_USE_SCENE_SERIALIZER)
	{
		s = VXEntities_INIT_Scene_Serializer();
		if (!s)
		{
			return s;
		}
	}
	if (flags & VXEntities_INIT_USE_MONO)
	{
		s = VXEntities_INIT_Mono();
		if (!s)
		{
			return s;
		}
	}
}

uint32_t VXEntities_GET_FLAGS()
{
	return flagsToUse;
}

bool VXEntities_FREE()
{
	if (flagsToUse & VXEntities_INIT_USE_ENTITIES && Scenes->size() != 0)
	{
		for (auto& e : *Scenes)
		{
			
			//Vertex::VXEntities_RemoveScene(e.second);
		}
	}

	delete Scenes;
	Scenes = nullptr;

	if (flagsToUse & VXEntities_INIT_USE_MONO)
	{
		Vertex::ScriptEngine::Shutdown();
	}

	return 1;
	
}

namespace Vertex
{
	Scene* VXEntities_MakeOrGetScene(std::string name)
	{
		// Check if the scene already exists in the map
		auto it = Scenes->find(name);
		if (it != Scenes->end()) {
			return it->second; // Return the existing scene
		}

		// If not, create a new scene
		Scene* newScene = new Scene(name);
		(*Scenes)[name] = newScene; // Add it to the map
		return newScene;
	}
	bool VXEntities_RemoveScene(Scene* scene)
	{
		if (scene == nullptr) return false;

		try
		{
			Scenes->erase(scene->name());
			delete scene;
			scene = nullptr;
			return true;
		}
		catch (const std::exception& ex)
		{
			VX_CORE_ASSERT(false, ex.what());
		}
		return false;
		
	}
	bool VXEntities_RemoveScene(std::string name)
	{
		try
		{
			Scene* scene = (*Scenes)[name];
			Scenes->erase(name);
			delete scene;
			return true;
		}
		catch (const std::exception& ex)
		{
			VX_CORE_ASSERT(false, ex.what());
		}
		return false;
		
	}
}