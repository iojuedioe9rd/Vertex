using System;
using System.Collections.Generic;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vertex;

namespace Sandbox
{
    public class Player : ENTBaseBoxCollier2D
    {
        public float Speed = 100.0f;
        public float Time = 0.0f;
        public float JumpTime = 0.0f;

        public Audio audio = new Audio("assets/music/idk.wav", true);

        public Player(string uuid) : base(uuid)
        {
        }

        public Player()
        {
        }

        protected override void OnCreate()
        {
            Logger.Info("Run 2");
            Type = BodyType.Dynamic;
            base.OnCreate();

            audio.Play();

        }

        protected override void OnDraw()
        {
            base.OnDraw();
            //Logger.Info("Run");
            Renderer2D.DrawQuad(Pos, Size, "assets/textures/Player.png", 1.0f, new Colour(1, 1, 1, 1));

            //Renderer2D.DrawQuad(Pos + new Vector3(0, Size.Y + 0.2f, 0),  // Position above the player
            //                    new Vector3(1, JumpTime / 10, 1),
            //                    new Colour(0, .6f, 0, 1));

            //Renderer2D.DrawQuad(Pos + new Vector3(-0.1f, -0.1f, 0),  // Slight offset for a shadow effect
            //                    Size,
            //                    new Colour(.4f, .4f, .4f, 1)); 
        }

        protected override void OnImGuiDraw()
        {
            ImGuiLink.Begin("Test", true);
            ImGuiLink.Text("Jump percent: %s", ((JumpTime / 30) * 100).ToString());
            ImGuiLink.End();

        }

        protected override void OnPhysUpdate(float ts)
        {
            base.OnPhysUpdate(ts);
        }

        protected override void OnUpdate(float ts)
        {
            Time += ts;

            //Logger.Info("Pos");
            //Logger.Info(Pos);

            //Logger.Info("Size");
            //Logger.Info(Size);

            base.OnUpdate(ts);
            float speed = Speed * ts;
            Vector2 velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            if(Input.IsKeyDown(KeyCode.Space))
            {
                JumpTime += ts * 2.5f;
            }

            if (Velocity >= Vector2.Zero && Input.IsKeyUp(KeyCode.Space))
            {
                velocity.Y += JumpTime * 11;
                JumpTime = 0;
            }

            velocity *= speed;

            ApplyLinearImpulse(velocity, true);
            ApplyLinearImpulse(Vector2.One / 1000, false);
        }
    }
}
