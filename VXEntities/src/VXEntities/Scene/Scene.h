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


		void OnUpdate(Timestep ts);
	private:
		std::vector<Entity*> m_Entitys;
		std::string m_name;

		friend class Entity;
	};

}