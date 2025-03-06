using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vertex;


public class MapBG : Entity
{
    protected override void OnDraw()
    {
        for (int i = 0; i < 100; i++)
        {
            Renderer2D.DrawQuad(new Vector3(100.0f * i, 0, -0.01f), new Vector3(100, 100, 1), new Colour(0, 0, 1, 1));
        }
        
    }
}

