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
        protected virtual void OnBeginContact(Entity ent) { }
        protected virtual void OnEndContact(Entity ent) { }


        public Vector3 Pos
        {
            get
            {
                
                InternalCalls.Entity_GetTranslation(UUID, out Vector3 result);

                if(result.X == (1 << 0xDEAD1) || result.Y == (2 << 0xDEAD2) || result.Z == (1 << 0xDEAD3))
                {
                    throw new Exception("DATA ERROR: " + result.ToString());
                }

                return result;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(UUID, ref value);
            }
        }


        public static Entity FindEntityByName(string name)
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

        public static T Instantiate<T>(string name, Vector3 pos = new Vector3(), Vector3? size = null, Vector3 rotation = new Vector3()) where T : Entity, new()
        {
            Vector3 finalSize = size ?? new Vector3(1, 1, 1);
            string instance = InternalCalls.Entity_Instantiate(typeof(T).Name, name, ref pos, ref finalSize, ref rotation);
            Entity entity = new Entity(instance);
            return entity.As<T>();
        }

        public static void RemoveENT(Entity entity)
        {
            InternalCalls.Entity_Remove(entity.UUID);
        }
    }
}
