#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace Vertex {

	class PathGenerator
	{
	public:
		PathGenerator(int gridWidth = 14, int gridHeight = 8);
		~PathGenerator();
		
		std::vector<glm::i32vec2> GeneratePath(int l, glm::i32vec2 offset);

		int getCellNeighbourValue(int x, int y);
		bool CellIsFree(int x, int y);


		glm::i32vec2 GetGridSize()
		{
			return glm::i32vec2(gridWidth, gridHeight);
		}

		std::vector<glm::i32vec2> GetPathCells();

		bool GenerateCroosroads();

	private:
		std::vector<glm::i32vec2> pathCells;

		


		

		int gridWidth, gridHeight = 64;

	};


}

