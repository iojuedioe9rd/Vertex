#pragma once

#include "Vertex/Core/Timestep.h"
#include <string>
#include <vector>  // Include this for std::vector
#include <Vertex/Renderer/Camera.h>
#include <Vertex/Events/Event.h>
#include "EditorCamera.h"

class b2World;

namespace Vertex
{
	class ENTPointCamera2D;
	class Entity;
	
	class Scene
	{
	public:
		Scene(std::string name);
		~Scene();

		std::string name()
		{
			return m_name;
		}

		template <typename T>
		T& CreateEntity(const std::string& name)
		{
			T* entity = new T(name, this);
			m_Entitys.push_back(entity);
			return *entity;
		}

		template <typename T>
		T& CreateEntity(T& entity)
		{
			T* newEntity = new T(entity);
			m_Entitys.push_back(newEntity);
			return *newEntity;
		}
		Entity* FindEntityByName(std::string_view name);
		bool RemoveEntity(Entity& entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		bool GetACameraInScene(Ref<Camera>* mainCamera, bool is2D, glm::mat4* cameraTransform = nullptr, ENTPointCamera2D** cam = nullptr, bool usePrimaryCam = true);
		void OnUpdate(Timestep ts);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnEvent(Event& e);
		void OnViewportResize(uint32_t width, uint32_t height);

		bool IsEditor() const { return m_IsEditor; }

		static Scene* Copy(Scene* other, std::string& name);

		bool IsRunning() const
		{
			return m_IsRunning;
		}

		void KillAllEntitys()
		{
			for (Entity* ent : m_Entitys)
			{
				if (RemoveEntity(*ent))
				{
					
					
				}
			}
		}

		std::vector<Entity*>::iterator begin() { return m_Entitys.begin(); }
		std::vector<Entity*>::iterator end() { return m_Entitys.end(); }
		std::vector<Entity*>::reverse_iterator rbegin() { return m_Entitys.rbegin(); }
		std::vector<Entity*>::reverse_iterator rend() { return m_Entitys.rend(); }

		std::vector<Entity*>::const_iterator begin() const { return m_Entitys.begin(); }
		std::vector<Entity*>::const_iterator end()	const { return m_Entitys.end(); }
		std::vector<Entity*>::const_reverse_iterator rbegin() const { return m_Entitys.rbegin(); }
		std::vector<Entity*>::const_reverse_iterator rend() const { return m_Entitys.rend(); }
	private:
		std::vector<Entity*> m_Entitys;
		std::string m_name;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsEditor = true;
		bool m_IsRunning = false;

		b2World* m_PhysicsWorld = nullptr;
		
		friend class Entity;
	};

}