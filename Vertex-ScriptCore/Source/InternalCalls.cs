using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace Vertex
{
    

    public static class InternalCalls
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorDot(ref Vector3 parameter);

    }
}
