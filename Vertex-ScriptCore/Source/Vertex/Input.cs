using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsKeyUp(KeyCode keycode)
        {
            return !IsKeyDown(keycode);
        }

        public static Vector2 GetMousePos()
        {
            Vector2 pos = new Vector2();
            InternalCalls.Input_GetMousePos(ref pos, true);
            return pos;
        }

        public static Vector2 GetMousePosWorld()
        {
            Vector2 pos = new Vector2();
            InternalCalls.Input_GetMousePos(ref pos, false);
            return pos;
        }

        public static bool IsMouseButtonPressed(MouseCode mouseCode)
        {
            return InternalCalls.Input_IsMouseButtonPressed(mouseCode);
        }
    }
}
