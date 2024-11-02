using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public struct Colour
    {
        private float X;
        private float Y;
        private float Z;
        private float W;

        public float R { get { return X; } set { X = value; } }
        public float G { get { return Y; } set { Y = value; } }
        public float B { get { return Z; } set { Z = value; } }
        public float A { get { return W; } set { W = value; } }

        public Colour(float R = 1, float G = 1, float B = 1, float A = 1)
        {
            X = R;
            Y = G;
            Z = B;
            W = A;
        }
            

    }
}
