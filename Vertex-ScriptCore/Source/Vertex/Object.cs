using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    using UUID = String;
    public class Object
    {
        public UUID UUID { get; internal set; }

        public Object()
        {
            UUID = InternalCalls.Object_GenerateUUID();
        }

        internal Object(UUID UUID)
        {
            this.UUID = UUID;
        }

    }
}
