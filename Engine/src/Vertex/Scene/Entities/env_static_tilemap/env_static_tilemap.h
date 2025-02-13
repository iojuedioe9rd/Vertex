#pragma once
#include "../../Entity.h"
#include <Vertex/Renderer/Texture.h>


namespace Vertex {

	class VERTEX_API ENTEnvStaticTilemap : public Entity
	{
	public:

		class VERTEX_API Tile : public Object
		{
		public:
			glm::i32vec2 pos = glm::i32vec2();
			glm::vec4 colour = glm::vec4(1,1,1,1);
			Ref<Texture2D> tex = nullptr;

			SerializationObject Serialize()
			{
				SerializationObject obj = SerializationObject();
				obj.Set("Position", SerializationType::Vector2, pos);
				obj.Set("Colour", SerializationType::Vector4, colour);
				if (tex)
				{
					obj.Set("TextureHandle", SerializationType::Uint64, tex->Handle.operator uint64_t());
				}
				return obj;
			}

			void DeSerialize(SerializationObject obj)
			{
				pos = obj.Get<glm::i32vec2>("Position", SerializationType::Vector2Int);
				colour = obj.Get<glm::vec4>("Colour", SerializationType::Vector4);
				if (obj.Contains("TextureHandle"))
				{
					uint64_t handle = obj.Get<uint64_t>("TextureHandle", SerializationType::Uint64);
					tex = std::dynamic_pointer_cast<Texture2D>(AssetManager::GetAsset(handle));
				}
			}
		};

		ENTEnvStaticTilemap()
		{

		}
		ENTEnvStaticTilemap(std::string name, Scene* scene) : Entity(name, scene)
		{

		}
		~ENTEnvStaticTilemap() = default;

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;

		void SetTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour, bool m_override = true);
		void AddTile(glm::i32vec2 pos, Ref<Texture2D> tex, glm::vec4 colour)
		{
			SetTile(pos, tex, colour, false);
		}
		virtual void EventH(Event& e) override
		{

		}
		
		bool RemoveTile(glm::i32vec2 pos);

		void Clear()
		{
			Tiles.clear();
		}

		virtual std::string GetEntName()  override
		{
			return "env_static_tilemap";
		}

		std::vector<Tile> GetTiles()
		{
			return Tiles;
		}

		virtual SerializationObject Serialize(bool isInSerializer = 0) override
		{
			SerializationObject obj = Entity::Serialize(isInSerializer);
			

			int i = -1;
			for (Tile& tile : Tiles)
			{
				i++;
				obj.Set("Tile" + std::to_string(i), SerializationType::SerializationObject, tile.Serialize());
			}

			obj.Set("TileCount", SerializationType::Int, i + 1);
			
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = 0) override
		{
			Entity::DeSerialize(obj, isInSerializer);
			
			for (int i = 0; i < obj.Get<int>("TileCount", SerializationType::Int); i++)
			{
				SerializationObject obj = obj.Get<SerializationObject>("Tile" + std::to_string(i), SerializationType::SerializationObject);
				Tile tile{};
				tile.DeSerialize(obj);
				Tiles.push_back(tile);
			}
			
			return true;
		}

	private:
		std::vector<Tile> Tiles;
	};


}