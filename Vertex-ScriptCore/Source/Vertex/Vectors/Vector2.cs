using System;

namespace Vertex
{
    public struct Vector2
    {
        public float X, Y;

        // Constructor
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        // Properties for magnitude and normalized vector
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y);
        public Vector2 Normalized => Magnitude > 0 ? this / Magnitude : new Vector2(0, 0);

        // Indexer
        public float this[int index]
        {
            get
            {
                if (index == 0) return X;
                if (index == 1) return Y;
                throw new IndexOutOfRangeException("Index out of range for Vector2");
            }
            set
            {
                if (index == 0) X = value;
                else if (index == 1) Y = value;
                else throw new IndexOutOfRangeException("Index out of range for Vector2");
            }
        }

        // Static properties for common vectors
        public static Vector2 Zero => new Vector2(0, 0);
        public static Vector2 One => new Vector2(1, 1);
        public static Vector2 Up => new Vector2(0, 1);
        public static Vector2 Down => new Vector2(0, -1);
        public static Vector2 Left => new Vector2(-1, 0);
        public static Vector2 Right => new Vector2(1, 0);

        // Addition and subtraction operators
        public static Vector2 operator +(Vector2 a, Vector2 b) => new Vector2(a.X + b.X, a.Y + b.Y);
        public static Vector2 operator -(Vector2 a, Vector2 b) => new Vector2(a.X - b.X, a.Y - b.Y);

        // Scalar multiplication and division
        public static Vector2 operator *(Vector2 a, float scalar) => new Vector2(a.X * scalar, a.Y * scalar);
        public static Vector2 operator /(Vector2 a, float scalar)
        {
            if (scalar == 0) throw new DivideByZeroException("Cannot divide by zero");
            return new Vector2(a.X / scalar, a.Y / scalar);
        }

        // Equality operators
        public static bool operator ==(Vector2 a, Vector2 b) => a.X == b.X && a.Y == b.Y;
        public static bool operator !=(Vector2 a, Vector2 b) => !(a == b);
        public static bool operator >(Vector2 a, Vector2 b) => (a.X > b.X) && (a.Y > b.Y);
        public static bool operator <(Vector2 a, Vector2 b) => (a.X < b.X) && (a.Y < b.Y);
        public static bool operator <=(Vector2 a, Vector2 b) => (a.X <= b.X) && (a.Y <= b.Y);
        public static bool operator >=(Vector2 a, Vector2 b) => (a.X >= b.X) && (a.Y >= b.Y);

        // Dot product
        public static float Dot(Vector2 a, Vector2 b) => a.X * b.X + a.Y * b.Y;

        // Cross product (returns scalar in 2D)
        public static float Cross(Vector2 a, Vector2 b) => a.X * b.Y - a.Y * b.X;

        // Angle between vectors
        public static float Angle(Vector2 a, Vector2 b)
        {
            float dot = Dot(a, b);
            float magProduct = a.Magnitude * b.Magnitude;
            return (float)Math.Acos(dot / magProduct) * (180f / (float)Math.PI); // Convert radians to degrees
        }

        // Override ToString for readable output
        public override string ToString() => $"({X}, {Y})";

        // Override Equals and GetHashCode
        public override bool Equals(object obj) => obj is Vector2 other && this == other;

        // Simple GetHashCode implementation
        public override int GetHashCode() => X.GetHashCode() ^ Y.GetHashCode();
    }
}
