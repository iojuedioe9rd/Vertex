#include "Vertex/Object/SerializationObject.h"
#pragma once
#include "Vertex/Core/Timestep.h"


#include <unordered_map>

namespace Vertex
{
    class Entity;
    class Scene;
    
#define VX_REGISTER_BEHAVIOUR(BehaviourClass, BehaviourName) \
    namespace { static BehaviourRegistrar<BehaviourClass> BehaviourClass##_##BehaviourName##_registrar(#BehaviourName); }

    class VERTEX_API Behaviour : public Object
    {
        
    public:
        using BehaviourCreateFunc = std::function<Behaviour* (Entity*)>;
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Timestep& ts) {}
        virtual void OnDraw() {}
        virtual void OnPhysUpdate(Timestep& deltaTime) {}

        virtual std::string GetTypeName() = 0;

        virtual SerializationObject Serialize()
        {
            SerializationObject obj = SerializationObject();
            obj.Set<UUID>("Behaviour", SerializationType::String, GetID());
            obj.Set("BehaviourType", SerializationType::String, GetTypeName());
            
            return obj;
        }

        virtual bool DeSerialize(SerializationObject obj)
        {
            SetID(obj.Get<std::string>("Behaviour", SerializationType::String));
            
            return true;
        }

		

    protected:
        Behaviour(Entity* entity) : m_Entity(entity) {}
        ~Behaviour() { m_Entity = nullptr; };

		

        Entity* m_Entity;

        static Behaviour* CreateBehaviour(const std::string& name, Entity* entity)
        {
            auto it = GetBehaviourMap().find(name);
            if (it != GetBehaviourMap().end())
                return it->second(entity);
            return nullptr;
        }

    private:

        template<typename T>
        static void RegisterBehaviour(const std::string& name)
        {
            GetBehaviourMap()[name] = [](Entity* entity) -> Behaviour* { return new T(entity); };
        }

        

        static std::unordered_map<std::string, BehaviourCreateFunc>& GetBehaviourMap()
        {
            static std::unordered_map<std::string, BehaviourCreateFunc> s_BehaviourMap;
            return s_BehaviourMap;
        }

        friend class Entity;
        friend class Scene;
		// HACK ↓
        template<typename T> friend class BehaviourRegistrar; 

    };

    template<typename T>
    class VERTEX_API BehaviourRegistrar
    {
    public:
        BehaviourRegistrar(const std::string& name)
        {
            Behaviour::RegisterBehaviour<T>(name);
        }
    };
}