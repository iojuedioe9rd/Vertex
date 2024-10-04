#pragma once

#include "Scene.h"
#include "Vertex/Object/Object.h"

#include <type_traits> // For std::is_base_of
#include <vector>
#include "Vertex/Core/Timestep.h"
#include <glm/glm.hpp>

namespace Vertex {



	class Entity : public Object
	{
	public:
		Entity() = default;
		Entity(std::string name, Scene* scene)
		{
			m_name = name;
			m_Scene = scene;
		}
		Entity(const Entity& other) = default;

		glm::vec3 pos = glm::vec3(0, 0, 0);
		glm::vec3 size = glm::vec3(1,1,1);
		glm::vec3 rotation = glm::vec3(0, 0, 0);

		std::string name()
		{
			return m_name;
		}

#pragma region IDK
		Entity* parent() { return m_parent; }

		bool isLoaded() { return m_isLoaded; }

		bool GetIsVisible()
		{
			return m_isVisible;
		}

		bool SetIsVisible(bool value)
		{
			m_isVisible = value;
			return value;
		}

		Entity& addChild(Entity* child)
		{
			child->m_parent = this;
			m_children.emplace_back(child);
			child->onAdded(m_Scene);
			return *child;
		}



		bool removeChild(Entity& child)
		{
			int index = 0;
			
			for (Entity* e : m_children)
			{

				if (e->GetID() == child.GetID())
				{
					m_children.erase(m_children.begin() + index);
					delete e;
					return true;
				}

				index++;
			}

			for (Entity* e : m_children)
			{
				bool v = e->removeChild(child);
				if (v)
				{
					return true;
				}
			}

			return false;
		}


		Entity* getEntityByName(std::string name)
		{
			if (m_name == name)
			{
				return this;
			}

			for (Entity* e : m_children)
			{
				Entity* result = e->getEntityByName(name);
				if (result != nullptr)
				{
					return result;
				}
			}

			return nullptr;
		}



#pragma endregion

#pragma region Procedures

		virtual void ImGuiRenderTime()
		{
			ImGuiRender();

			for (Entity* ent : m_children)
			{
				ent->ImGuiRenderTime();
			}
		}
		
		void UpdateTime(Timestep& ts)
		{
			Update(ts);

			for (Entity* ent : m_children)
			{
				ent->UpdateTime(ts);
			}
		}

		void DrawTime(Timestep& ts)
		{
			Draw(ts);

			for (Entity* ent : m_children)
			{
				ent->DrawTime(ts);
			}
		}

#pragma endregion

		virtual void Update(Timestep& ts) = NULL;
		virtual void Draw(Timestep& ts)   = NULL;
		virtual void ImGuiRender()        = NULL;
		virtual std::string GetEntName()  =	NULL;
		
	protected:
		Scene* m_Scene = nullptr;
		std::string m_name;

	private:
		std::vector<Entity*> m_children;
		Entity* m_parent = nullptr;
		bool m_isLoaded = false;
		
		bool m_isVisible = true;


		void onAdded(Scene* scene)
		{
			m_Scene = scene;
		}
		
	};
}

