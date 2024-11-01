using System;
using System.Runtime.Serialization;

namespace Vertex
{
    

    [Serializable]
    public struct Vector3 : IEquatable<Vector3>
    {
        // Fields
        public float X;
        public float Y;
        public float Z;

        // Constructor
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        // Properties
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        public float SqrMagnitude => X * X + Y * Y + Z * Z;
        public Vector3 Normalized => Magnitude > 0 ? this / Magnitude : Zero;

        public static Vector3 Zero => new Vector3(0, 0, 0);
        public static Vector3 One => new Vector3(1, 1, 1);

        // Indexer
        public float this[int index]
        {
            get
            {
                if (index == 0) return X;
                if (index == 1) return Y;
                if (index == 2) return Z;
                throw new IndexOutOfRangeException();
            }
            set
            {
                if (index == 0) X = value;
                else if (index == 1) Y = value;
                else if (index == 2) Z = value;
                else throw new IndexOutOfRangeException();
            }
        }

        // Operators
        public static Vector3 operator +(Vector3 a, Vector3 b) => new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        public static Vector3 operator -(Vector3 a, Vector3 b) => new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        // Multiplication of Vector3 by a float
        public static Vector3 operator *(Vector3 a, float scalar) => new Vector3(a.X * scalar, a.Y * scalar, a.Z * scalar);

        // Multiplication of a float by Vector3
        public static Vector3 operator *(float scalar, Vector3 a) => new Vector3(a.X * scalar, a.Y * scalar, a.Z * scalar);
        public static Vector3 operator /(Vector3 a, float scalar) => scalar != 0 ? new Vector3(a.X / scalar, a.Y / scalar, a.Z / scalar) : throw new DivideByZeroException();
        public static Vector3 operator -(Vector3 v) => new Vector3(-v.X, -v.Y, -v.Z);

        // Basic operations
        public float Dot(Vector3 other) => Dot(this, other);
        public static float Dot(Vector3 a, Vector3 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z;

        public static Vector3 Cross(Vector3 a, Vector3 b) => new Vector3(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        );

        // Interpolation
        public static Vector3 Lerp(Vector3 a, Vector3 b, float t) => a + (b - a) * Clamp01(t);

        // Min, Max, Clamp
        public static Vector3 Max(Vector3 a, Vector3 b) => new Vector3(Math.Max(a.X, b.X), Math.Max(a.Y, b.Y), Math.Max(a.Z, b.Z));
        public static Vector3 Min(Vector3 a, Vector3 b) => new Vector3(Math.Min(a.X, b.X), Math.Min(a.Y, b.Y), Math.Min(a.Z, b.Z));
        public static Vector3 Clamp(Vector3 value, Vector3 min, Vector3 max) => new Vector3(
            Clamp(value.X, min.X, max.X),
            Clamp(value.Y, min.Y, max.Y),
            Clamp(value.Z, min.Z, max.Z)
        );

        // Custom Clamp function
        private static float Clamp(float value, float min, float max) => value < min ? min : (value > max ? max : value);
        public static Vector3 Normalize(Vector3 vector) => vector / vector.Magnitude;

        // Additional functions
        public static Vector3 Reflect(Vector3 vector, Vector3 normal)
        {
            // Ensure the normal is normalized
            Vector3 normalizedNormal = normal.Normalized;

            // Calculate the reflection
            float dotProduct = Dot(vector, normalizedNormal);
            return vector - 2 * dotProduct * normalizedNormal;
        }
        public static Vector3 Project(Vector3 vector, Vector3 onNormal) => onNormal * (Dot(vector, onNormal) / onNormal.SqrMagnitude);

        // Override methods
        public override string ToString() => $"({X}, {Y}, {Z})";
        public override bool Equals(object obj) => obj is Vector3 other && Equals(other);
        public bool Equals(Vector3 other) => X == other.X && Y == other.Y && Z == other.Z;
        public override int GetHashCode() => (X.GetHashCode() * 397) ^ (Y.GetHashCode() * 397) ^ Z.GetHashCode();

        // Helper function
        private static float Clamp01(float value) => Clamp(value, 0f, 1f);
    }

}
