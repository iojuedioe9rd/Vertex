using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vertex;

namespace Sandbox
{
    public abstract class GunBase: Vertex.Object
    {
        public Player Player { get; set; }


        public abstract void Update(float ts);

        public abstract string GetName();

        public GunBase(Player player)
        {
            Player = player;
        }
    }
}
