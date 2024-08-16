#include "GridManager.h"
#include <Vertex/Core/Logger.h>

namespace Vertex {

	GridManager::GridManager(int gridWidth, int gridHeight)
	{
		this->gridWidth = gridWidth;
		this->gridHeight = gridHeight;
		pathGenerator = new PathGenerator(gridWidth, gridHeight);
		for (glm::highp_i32vec2 vec : pathGenerator->GeneratePath())
		{
			VX_INFO("x: {0}, y: {1}", vec.x, vec.y);
		}
	}

	GridManager::~GridManager()
	{
		delete pathGenerator;
	}

}