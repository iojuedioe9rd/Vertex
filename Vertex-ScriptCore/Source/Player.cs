using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vertex;

namespace Sandbox
{
    public class Player : ENTBaseBoxCollier2D
    {
        Texture2D tex;

        public Player(string uuid) : base(uuid)
        {
        }

        public Player()
        {
        }

        protected override void OnCreate()
        {
            tex = new Texture2D("assets/textures/Player.png");
            Type = BodyType.Dynamic;
            base.OnCreate();
        }

        protected override void OnDraw()
        {
            base.OnDraw();
            Renderer2D.DrawQuad(Pos, Size, tex, 1.0f, new Colour(1, 1, 1, 1));
        }

        protected override void OnPhysUpdate(float ts)
        {
            base.OnPhysUpdate(ts);
        }

        protected override void OnUpdate(float ts)
        {
            base.OnUpdate(ts);
            float speed = 100f * ts;
            Vector2 velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed;

            ApplyLinearImpulse(velocity, true);
            ApplyLinearImpulse(Vector2.One / 1000, false);
        }
    }
}
