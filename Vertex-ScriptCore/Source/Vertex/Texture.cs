using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class Texture2D
    {
        public string filename;

        public Texture2D(string filename)
        {
            this.filename = filename;
            InternalCalls.Texture2D_FromFilename(filename);
        }
    }
}
