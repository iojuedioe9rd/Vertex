#pragma once
#include "PathGenerator.h"
#include "TileMap.h"

namespace Vertex {

	class GridManager
	{
	public:
		
		GridManager(TileMap* tileMap, int l = 1, int gridWidth = 14, int gridHeight = 8, glm::i32vec2 offset = glm::i32vec2(0, 0));
		~GridManager();

		std::vector<glm::i32vec2> GetPathCells()
		{
			return pathGenerator->GetPathCells();
		}

		int gridWidth = 14;
		int gridHeight = 8;

	private:
		Ref<Texture2D> tex;
		Ref<Texture2D> tex2;
		Ref<Texture2D> tex3;
		PathGenerator* pathGenerator;
		TileMap* tileMap;
	};

}


