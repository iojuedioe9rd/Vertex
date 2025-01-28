using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    using AssetHandle = UInt64;
    public class Asset
    {
        public AssetHandle Handle { get; internal set; }

        public Asset()
        {
            Handle = InternalCalls.Object_GenerateIntUUID();
        }

        internal Asset(AssetHandle handle)
        {
            Handle = handle;
        }
    }
}
