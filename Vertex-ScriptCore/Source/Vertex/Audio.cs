using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class Audio : Asset
    {
        public Audio(string path, bool looped = false) : base()
        {
            Handle = InternalCalls.Audio_CreateSound(path, looped);
        }

        public void Play()
        {
            InternalCalls.Audio_PlaySound(Handle);
        }

        //~Audio()
        //{
        //    InternalCalls.Audio_Remove(Handle);
        //}
    }
}
