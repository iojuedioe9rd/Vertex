#pragma once
#include "VXEntities.h"
#include "../info_player/info_player.h"
#include <Vertex/Core/OrthographicCameraController.h>
#include "../../../resource.h"
#include "../prop_key_door/prop_key_door.h"



namespace Vertex {
	class ENTNpcEnemy;
	

	class ENTJsonStaticTilemap : public Entity
	{
	public:
		

		std::vector<std::vector<uint8_t>> GetAllWalls();

		virtual void Update(Timestep& ts) override {}
		virtual void Draw(Timestep& ts)   override {}
		std::vector<ENTNpcEnemy*> GetEnemys();
		ENTJsonStaticTilemap()
		{

		}
		ENTJsonStaticTilemap(std::string name, Scene* scene) : Entity(name, scene)
		{
			doorTex = new std::array<Ref<Texture2D>, 1>();
			textures[0] = Texture2D::Create("assets/textures/Checkerboard.png");
			textures[1] = Texture2D::Create("assets/textures/player/player.png");
			textures[2] = Texture2D::Create("map/Entity 425.png");
			textures[3] = Texture2D::Create("assets/textures/fire_column_medium_1.png");
			textures[4] = Texture2D::Create("PNG", IDB_PNG1);
			(*doorTex)[0] = Texture2D::Create("PNG", IDB_PNG3);


		}

		ENTPropKeyDoor* GetDoorOnPos(glm::vec2 pos, glm::vec2 size);

		~ENTJsonStaticTilemap();

		virtual std::string GetEntName() override
		{
			return "json_static_tilemap";
		}

		void Setup(ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam);

		std::thread* ReDoSetup(std::string filename, ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam, std::atomic<bool>& done);

		ENTEnvStaticTilemap* GetTilemap()
		{
			retASMName m_tilemap;
		}

		void RemoveEnemy(Entity* e);
		void RemoveEntity(Entity* e);
		virtual void ImGuiRender() override {};
		class jsonData : public Object
		{
		public:
			std::vector<int> rawData;
			uint16_t width;
			uint16_t height;
		};
		jsonData FromJSON(std::string& filename);

		Ref<Texture2D> textures[6];

		std::array<Ref<Texture2D>, 1>* doorTex;

		ENTEnvStaticTilemap* m_tilemap;

		OrthographicCameraController* m_cam;

		
		ENTInfoPlayer* m_player = nullptr;
	private:
		
		
		
	};

	

}