#include "GridManager.h"
#include <Vertex/Core/Logger.h>

namespace Vertex {

	GridManager::GridManager(TileMap* tileMap, int gridWidth, int gridHeight, glm::i32vec2 offset)
	{
		this->tex = Texture2D::Create("assets/textures/tile1.png");
		this->tex2 = Texture2D::Create("assets/textures/towerDefense_tile024.png");
		this->gridWidth = gridWidth;
		this->gridHeight = gridHeight;
		this->tileMap = tileMap;

		pathGenerator = new PathGenerator(gridWidth, gridHeight);

		int l =0;
		std::vector<glm::i32vec2> path = pathGenerator->GeneratePath(l, offset);
		pathGenerator->GenerateCroosroads();
		pathGenerator->GenerateCroosroads();
		
		path = pathGenerator->GetPathCells();

		for (glm::i32vec2 vec : path)
		{
			
			VX_INFO("x: {0}, y: {1}, z: {2}", vec.x, vec.y, pathGenerator->getCellNeighbourValue(vec.x, vec.y));
			tileMap->Add(vec, tex);
			
		}
		

		this->gridWidth = pathGenerator->GetGridSize().x;
		this->gridHeight = pathGenerator->GetGridSize().y;

		for (int x = 0; x < this->gridWidth; x++)
		{
			for (int y = 0; y < this->gridHeight; y++)
			{
				if (pathGenerator->CellIsFree(x + offset.x, y + offset.y))
				{
					tileMap->Add(glm::i32vec2(x + offset.x, y + offset.y), tex2);
				}
			}
		}

	}

	GridManager::~GridManager()
	{
		tileMap = 0;
		delete pathGenerator;
	}

}