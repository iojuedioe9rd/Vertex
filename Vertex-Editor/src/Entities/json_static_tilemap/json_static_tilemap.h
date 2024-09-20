#pragma once
#include "VXEntities.h"

namespace Vertex {

	class ENTJsonStaticTilemap : public Entity
	{
	public:
		

		virtual void Update(Timestep& ts) override {}
		virtual void Draw(Timestep& ts)   override {}

		ENTJsonStaticTilemap()
		{

		}
		ENTJsonStaticTilemap(std::string name, Scene* scene) : Entity(name, scene)
		{
			textures[0] = Texture2D::Create("assets/textures/Checkerboard.png");
		}
		~ENTJsonStaticTilemap();

		virtual std::string GetEntName() override
		{
			return "json_static_tilemap";
		}

		void Setup(ENTEnvStaticTilemap* tilemap);

		

		

	private:
		class jsonData : public Object
		{
		public:
			std::vector<int> rawData;
			uint16_t width;
			uint16_t height;
		};
		jsonData FromJSON(std::string filename);
		Ref<Texture2D> textures[2];
		ENTEnvStaticTilemap* m_tilemap;
	};

	

}