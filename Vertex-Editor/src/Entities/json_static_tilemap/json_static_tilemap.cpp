#include "json_static_tilemap.h"
#include "../../convert1DTo2D.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <Vertex/Core/Logger.h>

#define MAP_SIZE   16
#define MAP_WALL   1
#define MAP_PLAYER 2
using json = nlohmann::json;
using namespace nlohmann::literals;
namespace Vertex {

	

	
	

	void ENTJsonStaticTilemap::Setup(ENTEnvStaticTilemap* tilemap)
	{
		m_tilemap = tilemap;
		tilemap->Clear();

		jsonData jsonA = FromJSON("map/map1.json");
		
		// Use a dynamic 2D vector for storing the map data
		std::vector<std::vector<uint8_t>> data(jsonA.height, std::vector<uint8_t>(jsonA.width));

		// Fill the 2D vector using the values from the 1D array
		for (int row = 0; row < jsonA.height; ++row)
		{
			for (int col = 0; col < jsonA.width; ++col)
			{
				data[row][col] = jsonA.rawData[row * jsonA.width + col];
			}
		}


		for (int x = 0; x < jsonA.height; x++)
		{
			for (int y = 0; y < jsonA.width; y++)
			{
				switch (data[x][y]) {
				case MAP_WALL:
				{
					tilemap->AddTile(glm::i32vec2(x, y - (jsonA.height / 2)), textures[0], glm::vec4(1, 1, 1, 1));
				}
				}
			}
		}
		
	}

	ENTJsonStaticTilemap::jsonData ENTJsonStaticTilemap::FromJSON(std::string filename)
	{
		std::ifstream f(filename.c_str());
		json jsondata = json::parse(f);
		jsonData data = jsonData();
		f.close();

		json layers = jsondata["layers"];
		

		std::vector<int>  vectordata;

		for (json::iterator it = layers.begin(); it != layers.end(); ++it) {
			if ((*it)["name"].template get<std::string>() == "RawData")
			{
				data.width = (*it)["width"].template get<int>();
				data.height = (*it)["width"].template get<int>();
				for (auto& chunk : (*it)["chunks"].items())
				{
					for (auto& chunkData : chunk.value()["data"].items())
					{
						VX_INFO("{0}", chunkData.value().template get<int>());
						vectordata.emplace_back(chunkData.value().template get<int>());
					}
					
				}

			}
		}

		data.rawData = vectordata;

		

		return data;

	}

}