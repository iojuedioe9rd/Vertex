#pragma once
#include "../../Entity.h"
#include "../base_box_collider_2d/base_box_collider_2d.h"
#include "Vertex/Renderer/Texture.h"
#include <Vertex/AssetManager/AssetManager.h>

namespace Vertex {

	

	class VERTEX_API ENTPropStaticSprite : public ENTBaseBoxCollier2D
	{
	public:
		

		ENTPropStaticSprite()
		{

		}
		ENTPropStaticSprite(std::string name, Scene* scene) : ENTBaseBoxCollier2D(name, scene)
		{
			
		}
		~ENTPropStaticSprite();

		virtual void Update(Timestep& ts) override;
		virtual void PhysUpdate(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;
		virtual std::string GetEntName()  override
		{
			return "prop_static_sprite";
		}
		virtual void EventH(Event& e) override
		{

		}

		glm::vec4 colour = glm::vec4(1,1,1,1);
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;

		virtual SerializationObject Serialize(bool isInSerializer = false) override
		{
			SerializationObject obj = ENTBaseBoxCollier2D::Serialize();
			obj.Set("Colour", SerializationType::Vector4, colour);
			obj.Set("TilingFactor", SerializationType::Float, tilingFactor);
			if (texture && !isInSerializer)
			{
				obj.Set("TextureHandle", SerializationType::Uint64, texture->Handle.operator uint64_t());
			}
			
			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
		{
			ENTBaseBoxCollier2D::DeSerialize(obj);
			colour = obj.Get<glm::vec4>("Colour", SerializationType::Vector4);
			tilingFactor = obj.Get<float>("TilingFactor", SerializationType::Float);
			if (obj.Contains("TextureHandle"))
			{
				uint64_t handle = obj.Get<uint64_t>("TextureHandle", SerializationType::Uint64);
                texture = std::dynamic_pointer_cast<Texture2D>(AssetManager::GetAsset(handle));
			}
			return true;
		}

	private:

	};

	
	
	
}