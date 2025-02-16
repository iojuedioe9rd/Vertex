using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    using UUID = String;

    public class Entity : Object
    {
        public Entity(UUID uuid) : base(uuid)
        {
            
        }

        public Entity() : base(UUID.Empty)
        {
            
        }

        protected virtual void OnCreate()             { }
        protected virtual void OnUpdate(float ts)     { }
        protected virtual void OnDraw()               { }
        protected virtual void OnPhysUpdate(float ts) { }
        protected virtual void OnImGuiDraw() { }


        public Vector3 Pos
        {
            get
            {
                
                InternalCalls.Entity_GetTranslation(UUID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(UUID, ref value);
            }
        }


        public Entity FindEntityByName(string name)
        {
            string entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == string.Empty)
                return null;
            return new Entity(entityID);
        }

        public Vector3 Size = Vector3.One;
        public Vector3 Rotation { get; set; }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.GetScriptInstance(UUID);
            return instance as T;
        }
    }
}
