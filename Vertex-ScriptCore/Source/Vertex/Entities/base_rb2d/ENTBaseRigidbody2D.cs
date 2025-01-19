using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class ENTBaseRigidbody2D : Entity
    {
        public enum BodyType { Static = 0, Dynamic, Kinematic };
        public BodyType Type = BodyType.Static;
        public bool FixedRotation = false;

        private IntPtr bodyPtr = IntPtr.Zero;
        
        public Vector2 Velocity
        {
            get
            {
                Vector4 outV = new Vector4();
                InternalCalls.Rigidbody2D_GetVelocity(ref bodyPtr, ref outV);
                return outV.XY;
            }
        }


        public ENTBaseRigidbody2D(string uuid) : base(uuid)
        {
        }

        public ENTBaseRigidbody2D()
        {
        }

        protected override void OnUpdate(float ts)
        {
            
            Vector4 vec4 = new Vector4(Pos.X, Pos.Y, Pos.Z, 0);
            InternalCalls.Rigidbody2D_SetTransform(ref bodyPtr, ref vec4);

            Pos = new Vector3(vec4.X, vec4.Y, vec4.Z);
        }

        protected override void OnPhysUpdate(float ts)
        {
            Vector4 vec4 = new Vector4();
            InternalCalls.Rigidbody2D_GetTransform(ref bodyPtr, ref vec4);
            Pos = new Vector3(vec4.X, vec4.Y, Pos.Z);

        }

        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
        {
            InternalCalls.Rigidbody2D_ApplyLinearImpulse(ref bodyPtr, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2D_ApplyLinearImpulseToCenter(ref bodyPtr, ref impulse, wake);
        }
    }
}
