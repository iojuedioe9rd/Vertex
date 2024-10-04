#pragma once
#include "VXEntities.h"
#include "Pathfinding.h"
#include "../base_health_system/base_health_system.h"

namespace Vertex {
	class ENTBaseNPC : public ENTBaseHealthSystem<int, 0>
	{
	public:
		ENTBaseNPC()
		{

		}
		ENTBaseNPC(std::string name, Scene* scene) : ENTBaseHealthSystem(name, scene)
		{

		}
		~ENTBaseNPC();

	private:
		
		
		Pathfinding* base_npc_pathfinding = nullptr;
	protected:
		float speed = 4;
		Node* current = nullptr;
		void InIt(const std::vector<std::vector<uint8_t>>& grid);
		void UpdateNPC(Timestep& ts);
		bool base_npc_init = false;
		
	};

	
}