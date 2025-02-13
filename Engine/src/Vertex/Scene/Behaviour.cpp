#include "vxpch.h"
#include "Behaviour.h"
#include "Entity.h"

namespace Vertex
{
    Behaviour::Behaviour(Entity* entity) : m_Entity(entity)
    {
        
    }

    Behaviour* Behaviour::CreateBehaviour(const std::string& name, Entity* entity)
    {

        auto it = GetBehaviourMap().find(name);
        if (it != GetBehaviourMap().end())
            return it->second(entity);
        return nullptr;
    }
}

