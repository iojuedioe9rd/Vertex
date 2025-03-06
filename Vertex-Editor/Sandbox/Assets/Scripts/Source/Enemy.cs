using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Timer = Vertex.Timer;
using System.Threading.Tasks;
using Vertex;
using Sandbox;


public class Enemy : ENTBaseBoxCollier2D
{
    static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) 
    {
        Vector3 direction = target - current;
        float distance = direction.Magnitude;

	    if (distance <= maxDistanceDelta || direction.SqrMagnitude < 0.001f) {
		    return target;
	    }

        return current + (direction / distance) * maxDistanceDelta;  // Normalize safely
    }

    public enum EnemyAI_Type: int
    {
        Default = 0,
        Guner   = 1,
    }

    public Vector2 PointA;
    public Vector2 PointB;
    private EnemyAI_Type m_enemyAI;
    public int enemyAI_ID;
    private static int enemyAI_ID_MAX = 0;
    private bool movingToB = true; // Track movement direction
    public Timer timer = new Timer();
    public Timer timer2 = new Timer();

    protected override void OnCreate()
    {
        Type = BodyType.Dynamic;
        base.OnCreate();
        enemyAI_ID = enemyAI_ID_MAX;
        enemyAI_ID_MAX++;
        Density = 400;
        // 57.95
        switch (enemyAI_ID)
        {
            case 0:
                PointA = new Vector2(-4, 0.939999998f);
                PointB = new Vector2(4, 0.939999998f);
                m_enemyAI = EnemyAI_Type.Default;
                break;
            case 1:
                PointA = new Vector2(57.95f, 0.40f);
                PointB = new Vector2(77.45f, 0.40f);
                m_enemyAI = EnemyAI_Type.Default;
                break;
        }
    }

    protected override void OnBeginContact(Entity ent)
    {
        if(m_enemyAI == EnemyAI_Type.Guner)
            Entity.RemoveENT(this);
    }

    protected override void OnDraw()
    {
        Renderer2D.DrawQuad(Pos, Size, new Colour(1.0f, 0.0f, 0.0f, 1.0f));
    }

    protected override void OnPhysUpdate(float ts)
    {
        base.OnPhysUpdate(ts);
    }

    protected override void OnUpdate(float ts)
    {
        timer2.Update(ts);
        switch (m_enemyAI)
        {
            case EnemyAI_Type.Default:
                AI_Update_Default(ts);
                break;
            case EnemyAI_Type.Guner:
                AI_Update_Guner(ts);
                break;
        }
        base.OnUpdate(ts);
    }

    private void AI_Update_Default(float ts)
    {
        Vector3 target = movingToB ? new Vector3(PointB, Pos.Z) : new Vector3(PointA, Pos.Z);
        Pos = MoveTowards(Pos, target, 5.0f * ts);

        if ((Pos - target).SqrMagnitude < 0.001f)
        {
            movingToB = !movingToB; // Swap direction when reaching the target
        }
    }

    private void AI_Update_Guner(float ts)
    {
        Vector3 target = movingToB ? new Vector3(PointB, Pos.Z) : new Vector3(PointA, Pos.Z);
        Pos = MoveTowards(Pos, target, 5.0f * ts);

        if ((Pos - target).SqrMagnitude < 0.001f)
        {
            movingToB = !movingToB; // Swap direction when reaching the target
        }

        if(timer.GetSeconds() <= 1f)
        {
            timer.Update(ts);
            return;
        }

        Entity player = FindEntityByName("Player");

        if (player == null)
        {
            return;
        }

        Vector3 dir = (Pos - player.Pos).Normalized;
        EnemyGunerBu bu = Instantiate<EnemyGunerBu>("sdf", Pos + -(dir * 2));
        
        timer.Reset();
        if (bu == null)
        {
            return;
        }
        bu.Dir = (-dir).XY;
        bu.speed = (float)new Random((int)timer2.GetSeconds()).NextDouble() * 15.5f;
    }
}

public class EnemyGunerBu : ENTBaseBoxCollier2D
{
    public Vector2 Dir;
    public float speed = 5f;
    public Timer timer = new Timer();
    public float size = 1f;

    protected override void OnPhysUpdate(float ts)
    {
        base.OnPhysUpdate(ts);
    }

    protected override void OnDraw()
    {
        base.OnDraw();
        Renderer2D.DrawQuad(Pos, new Vector3(size, size, 1), "assets/textures/circle.png", 1.0f, new Colour(1, 1, 1, 1));
    }

    protected override void OnUpdate(float ts)
    {

        timer.Update(ts);
        Vector3 pos = Pos;
        pos.XY += Dir * speed * ts;
        Pos = pos;

        size = Player.Map(timer.GetSeconds(), 0.0f, 5.0f, 1.0f, 0.0f);

        if (timer.GetSeconds() >= 5.0f)  // Remove after 5 seconds
        {
            Entity.RemoveENT(this);
        }

        base.OnUpdate(ts);
    }
}
