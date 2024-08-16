#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace Vertex {

	class PathGenerator
	{
	public:
		PathGenerator(int gridWidth = 14, int gridHeight = 8);
		~PathGenerator();
		
		std::vector<glm::highp_i32vec2> GeneratePath();

	private:
		std::vector<glm::highp_i32vec2> pathCells;

		bool CellIsFree(int x, int y);

		int gridWidth, gridHeight = 64;

	};


}

