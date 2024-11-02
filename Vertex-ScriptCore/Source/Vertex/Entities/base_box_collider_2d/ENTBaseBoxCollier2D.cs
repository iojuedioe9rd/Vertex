using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class ENTBaseBoxCollier2D : ENTBaseRigidbody2D
    {

        private IntPtr RuntimeFixture;

        // TODO: move into physics material in the future maybe
        public float Density = 1.0f;
        public float Friction = 0.5f;
        public float Restitution = 0.0f;
        public float RestitutionThreshold = 0.5f;

        public ENTBaseBoxCollier2D(string uuid) : base(uuid)
        {
        }

        protected override void OnPhysUpdate(float ts)
        {
            base.OnPhysUpdate(ts);
        }

        protected override void OnUpdate(float ts)
        {
            base.OnUpdate(ts);
        }

        public ENTBaseBoxCollier2D()
        {
        }

        
    }
}
