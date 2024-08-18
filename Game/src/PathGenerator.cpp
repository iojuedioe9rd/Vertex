#include "PathGenerator.h"
#include "rand.h"
#include <vxpch.h>

namespace Vertex {

	PathGenerator::PathGenerator(int gridWidth, int gridHeight)
	{
		this->gridWidth = gridWidth;
		this->gridHeight = gridHeight;
	}

	PathGenerator::~PathGenerator()
	{
		
	}

	std::vector<glm::i32vec2> PathGenerator::GeneratePath(int l, glm::i32vec2 offset)
	{
		pathCells = std::vector<glm::i32vec2>();
		
		int y = (int)(gridHeight / 2);

		int x = 0;

		int width = (rand(l - 3, 3 + l) - l) + gridWidth;
		int height = (rand(l - 3, 3 + l) - l) + gridHeight;

		while (x < width)
		{
			pathCells.push_back(glm::highp_i32vec2(x + offset.x, y + offset.y));

			bool validMove = false;

			while (!validMove)
			{
				int move = rand(0, 3 + l);
				if (move == 0 + l || x % 2 == 0 || x > (width - 2))
				{
					x++;
					validMove = true;
				}
				else if (move == 1 + l && CellIsFree(x, y + 1) && y < height - 3)
				{
					y++;
					validMove = true;
				}
				else if (move == 2 + l && CellIsFree(x, y - 1) && y > 2)
				{
					y--;
					validMove = true;
				}
			}
		}

		return pathCells;
	}

	
#define CellIsTaken(x, y) !(CellIsFree(x, y))

	int PathGenerator::getCellNeighbourValue(int x, int y)
	{
		int returnValue = 0;

		if (CellIsTaken(x, y - 1))
		{
			returnValue += 1;
		}

		if (CellIsTaken(x - 1, y))
		{
			returnValue += 2;
		}
		if (CellIsTaken(x + 1, y))
		{
			returnValue += 4;
		}
		if (CellIsTaken(x, y + 1))
		{
			returnValue += 8;
		}

		return returnValue;
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

	std::vector<glm::i32vec2> PathGenerator::GetPathCells()
	{
		return pathCells;
	}

	bool PathGenerator::GenerateCroosroads()
	{
		int i = 0;
		for (glm::i32vec2 pathCell : pathCells)
		{
			i++;
			// I am sorry.
			int x = pathCell.x;
			int y = pathCell.y;

			

			if (x > gridWidth - 3) { continue; }
			if (y > gridHeight - 3) { continue; }
			if (gridHeight < 0)
			{
				if (y < gridHeight + 3) { continue; }
				
			}

			if (gridWidth < 0)
			{
				if (x < gridWidth + 3) { continue; }
			}

			if (CellIsFree(x, y + 3) && CellIsFree(x + 1, y + 3) && CellIsFree(x + 2, y + 3) &&
				CellIsFree(x, y + 2) && CellIsFree(x-1,y+2) && CellIsFree(x + 1, y + 2) && CellIsFree(x + 2, y + 2) &&
				CellIsFree(x, y + 1) && CellIsFree(x - 1, y + 1) && CellIsFree(x + 1, y + 1) && CellIsFree(x + 2, y + 1) &&
				CellIsFree(x + 1, y) && CellIsFree(x + 2, y) &&
				CellIsFree(x - 2, y - 3) && CellIsFree(x + 2, y - 1))
			{
				std::vector<glm::i32vec2> b;
				b.push_back(glm::i32vec2(x + 1, y));
				b.push_back(glm::i32vec2(x + 2, y));
				
				b.push_back(glm::i32vec2(x + 2, y + 1));
				b.push_back(glm::i32vec2(x, y + 1));

				b.push_back(glm::i32vec2(x + 2, y + 2));
				b.push_back(glm::i32vec2(x + 1, y + 2));
				b.push_back(glm::i32vec2(x, y + 2));
				b.push_back(glm::i32vec2(x, y));

				pathCells.insert(pathCells.begin() + i, b.begin(), b.end());

				

				return true;
			}
		}

		return false;
	}

	

}