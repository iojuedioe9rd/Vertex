#pragma once
#include "../../Entity.h"
#include "Vertex/Renderer/OrthographicCamera.h"
#include "Vertex/Core/Application.h"
#include "Vertex/Renderer/Camera.h"
#include "../../SceneCamera.h"

namespace Vertex {

	class VERTEX_API ENTPointCamera2D : public Entity
	{
	public:
		ENTPointCamera2D()
		{

		}
		ENTPointCamera2D(std::string name, Scene* scene) : Entity(name, scene)
		{
			camera.reset(new SceneCamera());
		}
		~ENTPointCamera2D()
		{
			
		}

		virtual void Update(Timestep& ts) override
		{

		}


		virtual void Draw(Timestep& ts)   override
		{

		}
		virtual std::string GetEntName()  override 
		{
			return "point_camera_2d";
		}
		virtual void EventH(Event& e) override
		{

		}

		
		

		
		bool isPrimary = true;
		Ref<SceneCamera> camera;

		bool FixedAspectRatio = false;

		virtual SerializationObject Serialize() override
		{
			SerializationObject obj = Entity::Serialize();
			
			obj.Set("IsPrimary", SerializationType::Bool, isPrimary);
			obj.Set("FixedAspectRatio", SerializationType::Bool, FixedAspectRatio);

			SerializationObject camObj = camera->Serialize();
			obj.Set("Camera", SerializationType::SerializationObject, camObj);

			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj) override
		{
			Entity::DeSerialize(obj);
			
			obj.Set("IsPrimary", SerializationType::Bool, isPrimary);
			obj.Set("FixedAspectRatio", SerializationType::Bool, FixedAspectRatio);

			SerializationObject camObj = obj.Get<SerializationObject>("Camera", SerializationType::SerializationObject);
			camera->DeSerialize(camObj);

			return true;
		}
	private:

	};


}