using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    using UUID = String;
    internal abstract class CPPEntity : Object
    {
        public CPPEntity() { }
        public CPPEntity(UUID uuid) : base(uuid) { }

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
        public Vector3 Size = Vector3.One;
        public Vector3 Rotation { get; set; }

        public abstract string GetENTName();
    }
}
