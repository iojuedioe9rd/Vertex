#pragma once

#include "Scene.h"
#include "Vertex/Object/Object.h"

#include <type_traits> // For std::is_base_of
#include <vector>
#include "Vertex/Core/Timestep.h"
#include <glm/glm.hpp>
#include "Vertex/Object/SerializationObject.h"
#include "SceneSerializer.h"
#include "Vertex/AssetManager/AssetManager.h"
#include "Vertex/Scene/Behaviour.h"



#define VX_ENT_GET_ENT_NAME_DEF(ent_name) \
virtual std::string GetEntName()  override\
{\
	return #ent_name;\
}\

// Macro to simplify entity registration
#define ENT_REGISTER_ENTITY(entityClass, entityName) \
    namespace { static EntityRegistrar<entityClass> entityClass##_##entityName##_registrar(#entityName); }


namespace Vertex {

	class Scene;

	class VERTEX_API Entity : public Object
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

		std::string& name()
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

		
		Behaviour* AddBehaviour(const std::string& name)
		{
			Behaviour* behaviour = Behaviour::CreateBehaviour(name, this);
			behaviour->OnCreate();

			m_Behaviours.push_back(behaviour);

			return behaviour;
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
		
		void UpdateTime(Timestep& ts)
		{
			Update(ts);
			for (size_t i = 0; i < m_Behaviours.size(); i++)
			{
				m_Behaviours[i]->OnUpdate(ts);
			}

			for (Entity* ent : m_children)
			{
				ent->UpdateTime(ts);
			}
		}

		Scene* GetScene()
		{
			return m_Scene;
		}

		void DrawTime(Timestep& ts)
		{
			if (this == nullptr || this == (Entity*)0xFFFFFFFFFFFFFFEF || this == (Entity*)0x0000019b651c64d0)
			{

				return;
			}

			try
			{
				Draw(ts);
				for (size_t i = 0; i < m_Behaviours.size(); i++)
				{
					m_Behaviours[i]->OnDraw();
				}

				for (Entity* ent : m_children)
				{
					ent->DrawTime(ts);
				}
			}
			catch (const std::exception& ex)
			{
				VX_INFO("Error: {0}", ex.what());
			}
			
		}
		void EventTime(Event& e)
		{
			EventH(e);

			for (Entity* ent : m_children)
			{
				ent->EventTime(e);
			}
		
		}

		void PhysUpdateTime(Timestep& ts)
		{
			PhysUpdate(ts);
			for (Entity* ent : m_children)
			{
				ent->PhysUpdateTime(ts);
			}

			for (size_t i = 0; i < m_Behaviours.size(); i++)
			{
				m_Behaviours[i]->OnPhysUpdate(ts);
			}
		}

#pragma endregion

		virtual void Update(Timestep& ts) = NULL;
		virtual void EventH(Event& e)
		{

		}
		virtual void Draw(Timestep& ts)   = NULL;
		virtual std::string GetEntName()  =	NULL;
		virtual void PhysUpdate(Timestep& ts)
		{

		}


		virtual void PostDeserialize()
		{

		}
		virtual SerializationObject Serialize()
		{
			SerializationObject obj = SerializationObject();
			obj.Set<UUID>("Entity", SerializationType::String, GetID());

			obj.Set("Tag", SerializationType::String, name());
			obj.Set("EntityType", SerializationType::String, GetEntName());
			obj.Set("Transform_Translation", SerializationType::Vector3, pos);
			obj.Set("Transform_Size", SerializationType::Vector3, size);
			obj.Set("Transform_Rotation", SerializationType::Vector3, rotation);

			SerializationObjectArray BehavioursArray = SerializationObjectArray();
			for (size_t i = 0; i < m_Behaviours.size(); i++)
			{
				BehavioursArray.Add(m_Behaviours[i]->Serialize());
			}

			obj.Set("Behaviours", SerializationType::SerializationObjectArray, BehavioursArray);

			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj)
		{
			SetID(obj.Get<std::string>("Entity", SerializationType::String));
			m_name = obj.Get<std::string>("Tag", SerializationType::String);
			pos = obj.Get<glm::vec3>("Transform_Translation", SerializationType::Vector3);
			size = obj.Get<glm::vec3>("Transform_Size", SerializationType::Vector3); // Updated for glm::vec3
			rotation = obj.Get<glm::vec3>("Transform_Rotation", SerializationType::Vector3); // Handle rotation as glm::vec3

			SerializationObjectArray BehavioursArray = obj.Get<SerializationObjectArray>("Behaviours", SerializationType::SerializationObjectArray);
			std::vector<SerializationObject> BehavioursArrayRawData = BehavioursArray.GetAll();

			for (size_t i = 0; i < BehavioursArrayRawData.size(); i++)
			{
				SerializationObject Data = BehavioursArrayRawData[i];
				Behaviour* behaviour = AddBehaviour(Data.Get<std::string>("BehaviourType", SerializationType::String));
				behaviour->SetID(Data.Get<std::string>("Behaviour", SerializationType::String));
				behaviour->DeSerialize(Data);

				m_Behaviours.push_back(behaviour);
			}


			return true;
		}

		void onAdded(Scene* scene)
		{
			m_Scene = scene;
		}
		

	protected:
		Scene* m_Scene = nullptr;
		std::string m_name;

	private:
		std::vector<Entity*> m_children;
		Entity* m_parent = nullptr;
		bool m_isLoaded = false;
		
		bool m_isVisible = true;

		std::vector<Behaviour*> m_Behaviours;
		

		friend class Scene;
		
	};

	class VERTEX_API EntityFactory {
	public:
		using EntityCreateFunc = std::function<Entity*(const std::string&, Scene*)>;

		static std::unordered_map<std::string, EntityCreateFunc>& GetRegistry() {
			static std::unordered_map<std::string, EntityCreateFunc> registry;
			return registry;
		}

		template<typename T>
		static void Register(const std::string& name) {
			GetRegistry()[name] = [](const std::string& entityName, Scene* scene) {
				return new T(entityName, scene);

				};
		}

		static Entity* CreateEntity(const std::string& name, const std::string& entityName, Scene* scene) {
			auto it = GetRegistry().find(name);
			if (it != GetRegistry().end()) {
				return it->second(entityName, scene);
			}
			return nullptr;
		}
	};


	template<typename T>
	class EntityRegistrar {
	public:
		EntityRegistrar(const std::string& name) {
			EntityFactory::Register<T>(name);
		}
	};
}

