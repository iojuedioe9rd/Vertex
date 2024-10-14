#include "../npc_enemy/npc_enemy.h"

#include "json_static_tilemap.h"
#include "../../convert1DTo2D.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <Vertex/Core/Logger.h>
#include "../npc_enemy/npc_enemy.h"
#include "../prop_key_door/prop_key_door.h"
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/transform.hpp>



#define MAP_SIZE        16
#define MAP_WALL        1
#define MAP_PLAYER      2
#define MAP_ENEMY       3
#define MAP_ENEMY_FAKE  4
#define MAP_DOOR_RED    5
#define MAP_DOOR_RED_UP 6
#define TILE_SIZE  32

using json = nlohmann::json;



namespace Vertex {
    std::vector<ENTNpcEnemy*> Enemys = std::vector<ENTNpcEnemy*>();
    std::vector<ENTPropKeyDoor*> Doors = std::vector<ENTPropKeyDoor*>();

    std::vector<std::vector<uint8_t>> ENTJsonStaticTilemap::GetAllWalls()
    {
        // Load and parse JSON data efficiently
        jsonData jsonA = FromJSON(std::string("map/map1.json"));

        // Preallocate the 2D vector for map data to avoid reallocations
        std::vector<std::vector<uint8_t>> data(jsonA.height, std::vector<uint8_t>(jsonA.width));

        // Convert the 1D array into a 2D array efficiently
        for (int row = 0; row < jsonA.height; ++row) {

            std::copy(jsonA.rawData.begin() + row * jsonA.width,
                jsonA.rawData.begin() + (row + 1) * jsonA.width,
                data[row].begin());
        }

        return data;
    }

    std::vector<ENTNpcEnemy*> ENTJsonStaticTilemap::GetEnemys()
    {
        return Enemys;
    }

    void CreateDoor(Entity* l, int x, int y, int height, int width, Scene* m_Scene, const std::string& colourName, ENTInfoPlayer::Key key, float rotation, std::array<Ref<Texture2D>, 1>* doorTex)
    {
        l = &m_Scene->CreateEntity<ENTPropKeyDoor>("Door " + colourName);
        l->pos = glm::vec3(x - (width / 2), y - (height / 2), 0);
        l->rotation.z = rotation;
        static_cast<ENTPropKeyDoor*>(l)->Setup(doorTex, key);
        Doors.push_back(static_cast<ENTPropKeyDoor*>(l));
    }

    void t(ENTJsonStaticTilemap* thisA, Scene* m_Scene, std::string fileName, std::atomic<bool>* done)
    {
        

        // Load and parse JSON data efficiently
        ENTJsonStaticTilemap::jsonData jsonA = thisA->FromJSON(fileName);

        // Preallocate the 2D vector for map data to avoid reallocations
        std::vector<std::vector<uint8_t>> data(jsonA.height, std::vector<uint8_t>(jsonA.width));

        // Convert the 1D array into a 2D array efficiently
        for (int row = 0; row < jsonA.height; ++row) {

            std::copy(jsonA.rawData.begin() + row * jsonA.width,
                jsonA.rawData.begin() + (row + 1) * jsonA.width,
                data[row].begin());
        }

        // Iterate and add tiles based on the parsed JSON data
        for (int y = 0; y < jsonA.height; ++y) {
            for (int x = 0; x < jsonA.width; ++x) {


                Entity* l;
                switch (data[y][x])
                {
                case MAP_WALL:
                    thisA->m_tilemap->AddTile(glm::i32vec2(x - (jsonA.width / 2), y - (jsonA.height / 2)), thisA->textures[0], glm::vec4(1, 1, 1, 1));
                    break;
                case MAP_PLAYER:
                    if (thisA->m_player != nullptr)
                    {
                        m_Scene->RemoveEntity(*thisA->m_player);
                    }
                    thisA->m_player = &m_Scene->CreateEntity<ENTInfoPlayer>("Player");
                    
                    // Adjust the player's position based on the actual grid size
                    thisA->m_player->pos = glm::vec3(
                        x - (jsonA.width / 2),  // Adjust for x position
                        y - (jsonA.height / 2), // Adjust for y position
                        0
                    );

                    VX_INFO("Player spawned at: x = {0}, y = {1}", thisA->m_player->pos.x, thisA->m_player->pos.y);
                    break;
                case MAP_ENEMY:
                    l = &m_Scene->CreateEntity<ENTNpcEnemy>("Enemy");

                    l->pos = glm::vec3(
                        x - (jsonA.width / 2),  // Adjust for x position
                        y - (jsonA.height / 2), // Adjust for y position
                        0
                    );
                    

                    Enemys.push_back((ENTNpcEnemy*)l);
                    break;

                case MAP_ENEMY_FAKE:
                    thisA->m_tilemap->AddTile(glm::i32vec2(x - (jsonA.width / 2), y - (jsonA.height / 2)), thisA->textures[2], glm::vec4(1, 1, 1, 1));
                    
                    break;
                case MAP_DOOR_RED:
                    CreateDoor(l, x, y, jsonA.height, jsonA.width, m_Scene, std::string("RED"), ENTInfoPlayer::Key::Red, -180, thisA->doorTex);

                    break;
                case MAP_DOOR_RED_UP:
                    CreateDoor(l, x, y, jsonA.height, jsonA.width, m_Scene, std::string("RED"), ENTInfoPlayer::Key::Red, 90, thisA->doorTex);

                    break;
                case 21:
                    VX_INFO("Bro get out!!!!!!!!!!!!!!!!!!!");
                    for (int y1 = 0; y1 < jsonA.height; ++y1)
                    {
                        for (int x1 = 0; x1 < jsonA.width; ++x1)
                        {

                            thisA->m_tilemap->AddTile(glm::i32vec2(x1, y1 - (jsonA.height / 2)), thisA->textures[0], glm::vec4(x1 / jsonA.width, y1 / jsonA.width, 1, 1));
                        }
                    }
                    break;
                }
            }
        }
        // l->Setup(m_player, textures[1], this);
        thisA->m_player->Setup(thisA->textures[1], thisA->textures[3], thisA->textures[4], thisA, thisA->m_cam);
        int i = 0;
        for (ENTNpcEnemy* l : Enemys)
        {
            l->Setup(thisA->m_player, thisA->textures[2], thisA, i);
            i++;
        }
        *done = true;
    }

    ENTPropKeyDoor* ENTJsonStaticTilemap::GetDoorOnPos(glm::vec2 pos, glm::vec2 size)
    {
        for (ENTPropKeyDoor* l : Doors)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), l->pos)
                * glm::rotate(glm::mat4(1.0f), glm::radians(l->rotation.z), { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { l->size.x, l->size.y, 1.0f });;
            glm::vec4 rect1 = glm::vec4(pos.x, pos.y, 0, size.x);
            glm::vec4 Doorpos = transform[0];
            glm::vec4 Doorsize = transform[1];


            if (!!(
                rect1.x < Doorpos.x + Doorsize.x &&
                rect1.x + rect1.w > Doorsize.x &&
                rect1.y < Doorsize.y + size.y &&
                rect1.y + size.y > Doorsize.y)
                ) {
                return l;

            }
        }
        return nullptr;
    }

    void ENTJsonStaticTilemap::Setup(ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam)
	{
		m_tilemap = tilemap;
		tilemap->Clear();

        m_cam = &cam;
        std::atomic<bool> done = false;
        std::thread* thread_object = new std::thread(t, this, m_Scene, std::string("map/map1.json"), &done);
        
        thread_object->join();
        delete thread_object;
		
	}

    std::thread* ENTJsonStaticTilemap::ReDoSetup(std::string filename, ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam, std::atomic<bool>& done)
    {
        m_tilemap = tilemap;
        tilemap->Clear();

        m_cam = &cam;

        for (ENTNpcEnemy* e : Enemys)
        {
            RemoveEnemy(e);
            m_Scene->RemoveEntity(*e);
        }

        for (ENTPropKeyDoor* d : Doors)
        {
            RemoveEntity(d);
            
        }

        std::thread* thread_object = new std::thread(t, this, m_Scene, filename, &done);

        return thread_object;

    }

    void ENTJsonStaticTilemap::RemoveEnemy(Entity* enemy)
    {
        for (size_t i = 0; i < Enemys.size(); i++)
        {
            if ((*Enemys[i]) == *enemy)
            {

                Enemys.erase(std::remove(Enemys.begin(), Enemys.end(), enemy), Enemys.end());
                
                
            }
            //int int_to_remove = n;
            //m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), int_to_remove), m_Entitys.end());
        }
    }

    void ENTJsonStaticTilemap::RemoveEntity(Entity* e)
    {
        for (size_t i = 0; i < Doors.size(); i++)
        {
            if ((*Doors[i]) == *e)
            {

                Doors.erase(std::remove(Doors.begin(), Doors.end(), e), Doors.end());
                m_Scene->RemoveEntity(*e);
                return;

            }
            //int int_to_remove = n;
            //m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), int_to_remove), m_Entitys.end());
        }

        for (size_t i = 0; i < Enemys.size(); i++)
        {
            if ((*Enemys[i]) == *e)
            {

                Enemys.erase(std::remove(Enemys.begin(), Enemys.end(), e), Enemys.end());
                m_Scene->RemoveEntity(*e);
                return;

            }
            //int int_to_remove = n;
            //m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), int_to_remove), m_Entitys.end());
        }
    }
    

    ENTJsonStaticTilemap::jsonData ENTJsonStaticTilemap::FromJSON(std::string& filename)
    {
        std::ifstream f(filename);
        if (!f.is_open()) {
            VX_ERROR("Failed to open file: {0}", filename);
            return {};
        }

        // Parse the JSON file
        json jsondata;
        f >> jsondata;
        f.close();

        // Initialize the jsonData structure
        jsonData data;

        // Cache references to frequently used JSON objects
        const auto& layers = jsondata.at("layers");

        // Variables to track the map's minimum and maximum coordinates
        int minX = 0, minY = 0, maxX = 0, maxY = 0;

        // First pass: calculate the map bounds based on the chunk coordinates
        for (const auto& layer : layers) {
            if (layer.at("name") == "RawData") {
                const auto& chunks = layer.at("chunks");
                for (const auto& chunk : chunks) {
                    int chunkX = chunk.at("x").get<int>();
                    int chunkY = chunk.at("y").get<int>();
                    int chunkWidth = chunk.at("width").get<int>();
                    int chunkHeight = chunk.at("height").get<int>();

                    // Update min/max values for x and y coordinates
                    minX = std::min(minX, chunkX);
                    minY = std::min(minY, chunkY);
                    maxX = std::max(maxX, chunkX + chunkWidth);
                    maxY = std::max(maxY, chunkY + chunkHeight);
                }
            }
        }

        // Calculate the width and height of the full map
        data.width = maxX - minX;
        data.height = maxY - minY;

        // Initialize the global map data with width and height
        data.rawData.resize(data.width * data.height, 0);  // Fill with 0 initially

        // Second pass: populate the rawData array with tile values
        for (const auto& layer : layers) {
            if (layer.at("name") == "RawData") {
                const auto& chunks = layer.at("chunks");
                for (const auto& chunk : chunks) {
                    int chunkX = chunk.at("x").get<int>();
                    int chunkY = chunk.at("y").get<int>();
                    int chunkWidth = chunk.at("width").get<int>();
                    int chunkHeight = chunk.at("height").get<int>();

                    const auto& chunkData = chunk.at("data");

                    // Iterate over each tile in the chunk
                    for (int row = 0; row < chunkHeight; ++row) {
                        for (int col = 0; col < chunkWidth; ++col) {
                            int tileValue = chunkData[row * chunkWidth + col].get<int>();

                            // Swap row and col to correct the orientation
                            int globalX = chunkX + col - minX;
                            int globalY = chunkY + row - minY;

                            // Ensure the position is within the map bounds
                            if (globalX >= 0 && globalX < data.width && globalY >= 0 && globalY < data.height) {
                                // Translate 2D coordinates to 1D index correctly (use y * width + x)
                                int globalIndex = globalY * data.width + globalX;

                                // Assign the tile value to the global map data
                                data.rawData[globalIndex] = tileValue;
                            }
                        }
                    }
                }
                break;  // Exit the loop once we've processed the "RawData" layer
            }
        }

        return data;
    }

}