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
     

        public List<GunBase> Guns = new List<GunBase>();

        public float Speed = 100.0f;
        public float Time = 0.0f;
        public float Time34 = 0.0f;
        public float JumpTime = 0.0f;

        public Audio audio = new Audio("assets/music/idk.wav", true);

        public UInt16 gunIndex = 0;

        public bool cooked = false;
        public Timer cookedTimer;

        public Player(string uuid) : base(uuid)
        {
        }

        public Player()
        {
        }

        protected override void OnBeginContact(Entity ent)
        {
            
        }

        public void OnLava()
        {
            cooked = true;
            cookedTimer.SetSeconds(0);
        }

        protected override void OnCreate()
        {
            Logger.Info("Run 2");
            Type = BodyType.Dynamic;
            base.OnCreate();
            gunIndex = 1;
            Guns.Add(new TestGun(this));
            Guns.Add(new TestGun(this));
            audio.Play();


            cookedTimer = new Timer();
        }

        public UInt16 GetGunIndex(GunBase gun)
        {
            for (UInt16 i = 0; i < Guns.Count; i++)
            {
                if(Guns[i] == gun)
                {
                    return (UInt16)(i + 1);
                }
            }

            return 0;
        }

        public GunBase GetGunAtIndex(UInt16 index)
        {
            return Guns[index - 1];
        }

        public static float Map(float value, float inMin, float inMax, float outMin, float outMax)
        {
            return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
        }

        protected override void OnDraw()
        {
            base.OnDraw();

            float offset1 = /*Map((float)Math.Sin(Time * 2.0f), -1, 1, 0, .1f)*/ 0;
            float offset2 = cooked ? -cookedTimer.GetSeconds() : Map((float)Math.Cos(Time34 * 3.0f), -1, 1, -0.05f, .1f);

            

            //Logger.Info("Run");
            Renderer2D.DrawQuad(Pos + new Vector3(0, offset2, 0), Size, "assets/textures/circle.png", 1.0f, new Colour(1, 0.44705882352f, 0, 1));
            Renderer2D.DrawQuad(Pos + new Vector3(0, 1 / 1.3f + offset1 + offset2, 0), Size / 1.5f, "assets/textures/cathead.png", 1.0f, new Colour(1, 1, 1, 1));

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
            
            ImGuiLink.Text("OS Info: %s", Environment.Version.ToString());
            ImGuiLink.Text("Jump percent: %s", ((JumpTime / 10) * 100).ToString());
            
            ImGuiLink.End();

        }

        protected override void OnPhysUpdate(float ts)
        {
            if(!cooked)
            {
                base.OnPhysUpdate(ts);
            }
            
        }

        protected override void OnUpdate(float ts)
        {
            if (!cooked)
            {
                base.OnUpdate(ts);
            }

            Time += ts * 2;
            Time34 += ts * 2.5f;

            //Logger.Info("Pos");
            //Logger.Info(Pos);

            Guns[gunIndex - 1].Update(ts);

            //Logger.Info("Size");
            //Logger.Info(Size);

            if(Pos.Y <= -4.10f)
            {
                cooked = true;
                
            }

            if(cooked)
            {
                cookedTimer.Update(ts / 2.0f);
                Vector3 newPos = Pos;

                newPos.Y = -4.10f;

                Pos = newPos;
                return;
            }

            
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

            if(Input.IsKeyDown(KeyCode.D1) && Guns.Count > 0)
            {
                gunIndex = 1;
            }

            if (Input.IsKeyDown(KeyCode.D2) && Guns.Count > 1)
            {
                gunIndex = 2;
            }

            if (Input.IsKeyDown(KeyCode.D3) && Guns.Count > 2)
            {
                gunIndex = 3;
            }

            if (Input.IsKeyDown(KeyCode.D4) && Guns.Count > 3)
            {
                gunIndex = 4;
            }

            if (Input.IsKeyDown(KeyCode.D5) && Guns.Count > 4)
            {
                gunIndex = 5;
            }

            if (Input.IsKeyDown(KeyCode.D6) && Guns.Count > 5)
            {
                gunIndex = 6;
            }

            if (Input.IsKeyDown(KeyCode.D7) && Guns.Count > 6)
            {
                gunIndex = 7;
            }

            if (Input.IsKeyDown(KeyCode.D8) && Guns.Count > 7)
            {
                gunIndex = 8;
            }

            if (Input.IsKeyDown(KeyCode.D9) && Guns.Count > 8)
            {
                gunIndex = 9;
            }

            if (Input.IsKeyDown(KeyCode.D0) && Guns.Count > 9)
            {
                gunIndex = 10;
            }
        }
    }
}
