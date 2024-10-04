#include "vxpch.h"



#include "Scene.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/glm.hpp>




#include "Entity.h"


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
				delete &entity;
				return true;
			}
				//int int_to_remove = n;
				//m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), int_to_remove), m_Entitys.end());
		}

		return 0;


	}
	void Scene::OnImGuiRender()
	{
		for (Entity* ent : m_Entitys)
		{
			ent->ImGuiRenderTime();
		}
	}

	void Scene::OnUpdate(Timestep& ts)
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

}