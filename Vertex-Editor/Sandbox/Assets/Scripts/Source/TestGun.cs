using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vertex;

namespace Sandbox
{
    public class TestGun : GunBase
    {
        Timer timer;

        public TestGun(Player player) : base(player)
        {
            timer = new Timer();
        }

        public override string GetName()
        {
            return "Test Gun";
        }

        public override void Update(float ts)
        {
            timer.Update(ts);

            if (timer.GetSeconds() >= 0.3f && Input.IsMouseButtonPressed(MouseCode.ButtonLeft))
            {
                Vector2 dir = Input.GetMousePosWorld().Normalized;

                Logger.Info(dir);

                EnemyGunerBu bu = Entity.Instantiate<EnemyGunerBu>("rse", Player.Pos + new Vector3(dir * 2, 0));
                bu.Dir = dir;
                bu.speed = (float)new Random((int)timer.GetSeconds()).NextDouble() * 6.969f;

                timer.Reset();
            }
        }
    }
}
