#pragma once
#include "../../Entity.h"
#include "Vertex/Renderer/OrthographicCamera.h"
#include "Vertex/Core/Application.h"

namespace Vertex {

	class ENTPointCamera : public Entity
	{
	public:
		ENTPointCamera()
		{

		}
		ENTPointCamera(std::string name, Scene* scene) : Entity(name, scene)
		{

		}
		~ENTPointCamera()
		{
			if (orthographicCamera != nullptr)
			{
				delete orthographicCamera;
			}
		}

		virtual void Update(Timestep& ts) override
		{
			if (isAOrthographicCamera && orthographicCamera == nullptr)
			{
				float aspectRatio = Application::GetWindowSize().x / Application::GetWindowSize().y;
				orthographicCamera = new OrthographicCamera(-aspectRatio, aspectRatio, -1, 1);
				
			}
		}
		virtual void Draw(Timestep& ts)   override
		{

		}
		virtual std::string GetEntName()  override 
		{
			return "point_camera";
		}

		OrthographicCamera* orthographicCamera = nullptr;

		bool isAOrthographicCamera = true; 
		bool isPrimary = true;
		

	private:

	};


}