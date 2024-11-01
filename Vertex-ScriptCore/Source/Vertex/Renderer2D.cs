using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class Renderer2D
    {
        public static void DrawQuad(Vector3 pos, Vector3 size, Colour colour)
        {
            InternalCalls.Renderer2D_DrawQuad(ref pos, ref size, ref colour);
        }

        public static void DrawQuad(Vector3 pos, Vector3 size, Texture2D texture2D, float tilingFactor, Colour colour)
        {
            InternalCalls.Renderer2D_DrawQuadTex(ref pos, ref size, texture2D.filename, tilingFactor, ref colour);
        }
    }
}
