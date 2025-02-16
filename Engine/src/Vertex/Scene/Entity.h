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
}

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

        // Member variables
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::vec3 size = glm::vec3(1, 1, 1);
        glm::vec3 rotation = glm::vec3(0, 0, 0); // Fixed: rotation reintroduced

        std::string& name()
        {
            return m_name; // Return the name of the entity
        }

        // Getter for 'name'
        std::string GetName() const { return m_name; }

        // Single Behaviour instead of a vector
        Behaviour* m_Behaviour = nullptr; // Fixed: Changed to single Behaviour

        // Other member functions...
        Entity* parent() { return m_parent; }
        bool isLoaded() { return m_isLoaded; }
        bool GetIsVisible() { return m_isVisible; }
        bool SetIsVisible(bool value) { m_isVisible = value; return value; }

        Entity& addChild(Entity* child)
        {
            child->m_parent = this;
            m_children.emplace_back(child);
            child->onAdded(m_Scene);
            return *child;
        }

        // Add single behaviour instead of multiple
        Behaviour* AddBehaviour(const std::string& name)
        {
            if (m_Behaviour != nullptr) { return m_Behaviour; } // Prevent adding more than one behaviour

            m_Behaviour = Behaviour::CreateBehaviour(name, this);
            if (m_Behaviour == nullptr) return nullptr;
            m_Behaviour->OnCreate();
            return m_Behaviour;
        }

        // More member functions...
        void UpdateTime(Timestep& ts)
        {
            Update(ts);
            if (m_Behaviour) {
                m_Behaviour->OnUpdate(ts);
            }
            for (Entity* ent : m_children)
            {
                ent->UpdateTime(ts);
            }
        }

        void OnCreateTime()
        {
            OnCreate();
            if (m_Behaviour) {
                m_Behaviour->OnCreate();
            }
            for (Entity* ent : m_children)
            {
                ent->OnCreateTime();
            }
        }

        void OnDestroyTime()
        {
            OnDestroy();
            if (m_Behaviour) {
                m_Behaviour->OnDestroy();
            }
            for (Entity* ent : m_children)
            {
                ent->OnDestroyTime();
            }
        }

        void OnImGuiDrawTime()
        {
            OnImGuiDraw();
            for (Entity* ent : m_children)
            {
                ent->OnImGuiDraw();
            }
        }

        Scene* GetScene() { return m_Scene; }

        void DrawTime(Timestep& ts)
        {
            try
            {
                Draw(ts);
                if (m_Behaviour) {
                    m_Behaviour->OnDraw();
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

        Behaviour* GetBehaviour()
        {
            return m_Behaviour;
        }

        void PhysUpdateTime(Timestep& ts)
        {
            PhysUpdate(ts);
            for (Entity* ent : m_children)
            {
                ent->PhysUpdateTime(ts);
            }
            if (m_Behaviour) {
                m_Behaviour->OnPhysUpdate(ts);
            }
        }

        void PostDeserializeTime()
        {
            PostDeserialize();
            for (Entity* ent : m_children)
            {
                ent->PostDeserializeTime();
            }
            if (m_Behaviour)
            {
                m_Behaviour->PostDeSerialize();
            }
        }

        virtual void Update(Timestep& ts) = NULL;
        virtual void EventH(Event& e) {}
        virtual void OnImGuiDraw() {}
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void Draw(Timestep& ts) = NULL;
        virtual std::string GetEntName() = NULL;
        virtual void PhysUpdate(Timestep& ts) {}

        virtual void PostDeserialize() {}

        // Serialize method
        virtual SerializationObject Serialize(bool isInSerializer = false)
        {
            SerializationObject obj = SerializationObject();
            obj.Set<UUID>("Entity", SerializationType::String, GetID());

            obj.Set("Tag", SerializationType::String, name());
            obj.Set("EntityType", SerializationType::String, GetEntName());
            obj.Set("Transform_Translation", SerializationType::Vector3, pos);
            obj.Set("Transform_Size", SerializationType::Vector3, size);
            obj.Set("Transform_Rotation", SerializationType::Vector3, rotation);

            // Serialize the single behaviour
            if (m_Behaviour) {
                obj.Set("Behaviour", SerializationType::SerializationObject, m_Behaviour->Serialize(isInSerializer));
            }

            return obj;
        }

        // Deserialize method
        virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false)
        {
            SetID(obj.Get<std::string>("Entity", SerializationType::String));
            m_name = obj.Get<std::string>("Tag", SerializationType::String);
            pos = obj.Get<glm::vec3>("Transform_Translation", SerializationType::Vector3);
            size = obj.Get<glm::vec3>("Transform_Size", SerializationType::Vector3); // Updated for glm::vec3
            rotation = obj.Get<glm::vec3>("Transform_Rotation", SerializationType::Vector3); // Handle rotation as glm::vec3

            // Deserialize the single behaviour
            if (obj.Contains("Behaviour")) {
                SerializationObject behaviourData = obj.Get<SerializationObject>("Behaviour", SerializationType::SerializationObject);
                m_Behaviour = Behaviour::CreateBehaviour(behaviourData.Get<std::string>("BehaviourType", SerializationType::String), this);
                if (m_Behaviour) {
                    m_Behaviour->DeSerialize(behaviourData, isInSerializer);
                }
            }

            return true;
        }

        void onAdded(Scene* scene)
        {
            m_Scene = scene;
        }

    protected:
        Scene* m_Scene = nullptr;
        std::string m_name = "";

    private:
        std::vector<Entity*> m_children;
        Entity* m_parent = nullptr;
        bool m_isLoaded = false;
        bool m_isVisible = true;

        friend class Scene;
        friend class Behaviour;
    };

    class VERTEX_API EntityFactory {
    public:
        using EntityCreateFunc = std::function<Entity* (const std::string&, Scene*)>;

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
