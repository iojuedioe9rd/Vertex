#pragma once
#include "VXEntities.h"
#include "../info_player/info_player.h"
#include <Vertex/Core/OrthographicCameraController.h>
namespace Vertex {

	

	class ENTJsonStaticTilemap : public Entity
	{
	public:
		

		std::vector<std::vector<uint8_t>> GetAllWalls();

		virtual void Update(Timestep& ts) override {}
		virtual void Draw(Timestep& ts)   override {}

		ENTJsonStaticTilemap()
		{

		}
		ENTJsonStaticTilemap(std::string name, Scene* scene) : Entity(name, scene)
		{
			textures[0] = Texture2D::Create("assets/textures/Checkerboard.png");
			textures[1] = Texture2D::Create("assets/textures/player/player.png");
			textures[2] = Texture2D::Create("map/Entity 425.png");
			textures[3] = Texture2D::Create("assets/textures/Checkerboard.png");
		}
		~ENTJsonStaticTilemap();

		virtual std::string GetEntName() override
		{
			return "json_static_tilemap";
		}

		void Setup(ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam);

		void ReDoSetup(std::string filename, ENTEnvStaticTilemap* tilemap, OrthographicCameraController& cam);

		void RemoveEnemy(Entity* e);
		virtual void ImGuiRender() override {};
		class jsonData : public Object
		{
		public:
			std::vector<int> rawData;
			uint16_t width;
			uint16_t height;
		};
		jsonData FromJSON(std::string& filename);

		Ref<Texture2D> textures[4];
		ENTEnvStaticTilemap* m_tilemap;

		OrthographicCameraController* m_cam;

		
		ENTInfoPlayer* m_player = nullptr;
	private:
		
		
		
	};

	

}