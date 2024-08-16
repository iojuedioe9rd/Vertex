#pragma once
#include "PathGenerator.h"


namespace Vertex {

	class GridManager
	{
	public:
		GridManager(int gridWidth = 14, int gridHeight = 8);
		~GridManager();


		int gridWidth = 14;
		int gridHeight = 8;

	private:
		PathGenerator* pathGenerator;
	};

}


