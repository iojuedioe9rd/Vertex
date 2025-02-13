#pragma once
#include <Vertex/Scene/Behaviour.h>



namespace Vertex
{
    using LuaScriptHandle = IntUUID;

	class BEHAVIOURLuaScript : public Behaviour
	{
	public:

        BEHAVIOURLuaScript(Entity* entity) : Behaviour(entity)
        {
            m_ScriptHandle = LuaScriptHandle(0);
        }
        ~BEHAVIOURLuaScript() {}

        virtual void OnCreate() override;
        virtual void OnDestroy() override;
        virtual void OnUpdate(Timestep& ts) override;

        virtual void ImGuiDrawProperties() override;

        virtual Entity* GetEntity() { return m_Entity; }

        virtual std::string GetTypeName() override
        {
            return "lua_script";
        }

        virtual SerializationObject Serialize(bool isInSerializer = false) override
        {
            SerializationObject obj = Behaviour::Serialize(isInSerializer);
            
            obj.Set("filepath", SerializationType::String, filepath.string());
            
            

            return obj;
        }

        virtual bool DeSerialize(SerializationObject obj, bool isInSerializer = false) override
        {
            Behaviour::DeSerialize(obj, isInSerializer);

            filepath = std::fs::path(obj.Get<std::string>("filepath", SerializationType::String));

            

            return true;
        }

        std::fs::path filepath;

    private:
        LuaScriptHandle m_ScriptHandle;
        



        friend class Entity;
        friend class Scene;
	};
}