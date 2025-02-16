using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace Vertex
{
    

    public static class InternalCalls
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorDot(ref Vector3 parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BeginLog(int flags);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void LogMsg(string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EndLog();

        #region Rendering

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Renderer2D_DrawQuad(ref Vector3 pos, ref Vector3 size, ref Colour colour);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Renderer2D_DrawQuadTex(ref Vector3 pos, ref Vector3 size, string textureFilename, float tilingFactor, ref Colour tintColour);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Texture2D_FromFilename(string filename);

        #endregion

        #region Entity

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetTranslation(string entityID, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetTranslation(string entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetRotation(string entityID, out Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetRotation(string entityID, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_FindEntityByName(string name);

        #endregion

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);

        #region RB 2D Entity

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Rigidbody2D_ApplyLinearImpulse(ref IntPtr rigidbody2DID, ref Vector2 impulse, ref Vector2 point, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Rigidbody2D_ApplyLinearImpulseToCenter(ref IntPtr rigidbody2DID, ref Vector2 impulse, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Rigidbody2D_GetTransform(ref IntPtr rigidbody2DID, ref Vector4 transform2D);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Rigidbody2D_SetTransform(ref IntPtr rigidbody2DID, ref Vector4 transform2D);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Rigidbody2D_GetVelocity(ref IntPtr rigidbody2DID, ref Vector4 transform2D);

        #endregion

        #region Object

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Object_GenerateUUID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static UInt64 Object_GenerateIntUUID();

        #endregion

        #region Audio
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static UInt64 Audio_CreateSound(string filename, bool looped);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Audio_PlaySound(UInt64 soundID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Audio_StopSound(UInt64 soundID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Audio_SetSoundVolume(UInt64 soundID, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Audio_SetSoundLooped(UInt64 soundID, bool looped);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Audio_SetSoundPitch(UInt64 soundID, float pitch);




        #endregion

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(string entityID);

        #region ImGui
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ImGui_Begin(string name, ref bool open);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_End();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ImGui_Text(string[] fmt);
        #endregion
    }
}
