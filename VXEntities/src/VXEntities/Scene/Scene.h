#pragma once

#include "Vertex/Core/Timestep.h"
#include <string>
#include <vector>  // Include this for std::vector

namespace Vertex
{

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


		bool RemoveEntity(Entity& entity);
		

		void OnUpdate(Timestep& ts);

		void OnImGuiRender();

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

		friend class Entity;
	};

}