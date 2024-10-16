#include "vxpch.h"



#include "Scene.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/glm.hpp>




#include "Entity.h"
#include "Entities/point_camera_2d/point_camera_2d.h"
#include <glm/ext/matrix_transform.hpp>


namespace Vertex {

	static void DoMath(const glm::mat4& transform)
	{

	}

	Scene::Scene(std::string name)
	{
		

		m_name = name;
		
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
		for (Entity* ent : m_Entitys)
		{
			delete ent;
		}
	}

	


	bool Scene::RemoveEntity(Entity& entity)
	{
		for (size_t i = 0; i < m_Entitys.size(); i++)
		{
			if ((*m_Entitys[i]) == entity)
			{

				m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), &entity), m_Entitys.end());
				delete& entity;
				return true;
			}
			//int int_to_remove = n;
			//m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), int_to_remove), m_Entitys.end());
		}
		return 0;
	}

	bool Scene::GetACameraInScene(Ref<Camera>* mainCamera, bool is2D, glm::mat4* cameraTransform, bool usePrimaryCam)
	{
		for (Entity* ent : m_Entitys)
		{
			if (is2D && ent->GetEntName() == "point_camera_2d")
			{
				ENTPointCamera2D* camera2D = dynamic_cast<ENTPointCamera2D*>(ent);
				
				if (0 || camera2D->isPrimary) // Optionally check if it's the primary camera
				{
					*mainCamera = camera2D->camera; // Set the main camera
					if (cameraTransform) // If the transform pointer is provided, update it
					{
						glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera2D->pos)
							* glm::rotate(glm::mat4(1.0f), glm::radians(camera2D->rotation.z), { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { camera2D->size.x, camera2D->size.y, 1.0f });
						*cameraTransform = transform;
					}
					return true; // Successfully found a 2D camera
				}
			}
		}
		return false;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->UpdateTime(ts);
		}

		for (Entity* ent : m_Entitys)
		{
			ent->DrawTime(ts);
		}

	}

	void Scene::OnEvent(Event& e)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->EventH(e);
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		for (Entity* ent : m_Entitys)
		{
			if (ent->GetEntName() == "point_camera_2d")
			{
				ENTPointCamera2D* camera2D = dynamic_cast<ENTPointCamera2D*>(ent);

				if (!camera2D->FixedAspectRatio)
				{
					camera2D->camera->SetViewportSize(width, height);
				}
				
			}
		}
	}

}