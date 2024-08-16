#include "PathGenerator.h"
#include "rand.h"

namespace Vertex {

	PathGenerator::PathGenerator(int gridWidth, int gridHeight)
	{
		this->gridWidth = gridWidth;
		this->gridHeight = gridHeight;
	}

	PathGenerator::~PathGenerator()
	{
		pathCells.clear();
	}

	std::vector<glm::highp_i32vec2> PathGenerator::GeneratePath()
	{
		pathCells = std::vector<glm::highp_i32vec2>();
		
		int y = (int)(gridHeight / 2);

		int x = 0;

		while (x < gridWidth)
		{
			pathCells.push_back(glm::highp_i32vec2(x, y));

			bool validMove = false;

			while (!validMove)
			{
				int move = rand(0, 3);
				if (move == 0 || x % 2 == 0)
				{
					x++;
					validMove = true;
				}
				else if (move == 1 && CellIsFree(x, y + 1))
				{
					y++;
					validMove = true;
				}
				else if (move == 2 && CellIsFree(x, y - 1))
				{
					y--;
					validMove = true;
				}
			}
		}

		return pathCells;
	}

	bool PathGenerator::CellIsFree(int x, int y)
	{
		bool free = true;

		for (glm::highp_i32vec2 vec : pathCells)
		{
			if (vec.x == x && vec.y == y) {
				free = false;
			}
		}

		return free;
	}

	

}