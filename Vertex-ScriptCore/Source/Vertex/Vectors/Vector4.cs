using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vertex
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        // Constructor
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        // Properties for magnitude and normalized vector
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y + Z * Z + W * W);
        public Vector4 Normalized => Magnitude > 0 ? this / Magnitude : new Vector4(0, 0, 0, 0);

        // Indexer
        public float this[int index]
        {
            get
            {
                if (index == 0) return X;
                if (index == 1) return Y;
                if (index == 2) return Z;
                if (index == 3) return W;
                throw new IndexOutOfRangeException("Index out of range for Vector4");
            }
            set
            {
                if (index == 0) X = value;
                else if (index == 1) Y = value;
                else if (index == 2) Z = value;
                else if (index == 3) W = value;
                else throw new IndexOutOfRangeException("Index out of range for Vector4");
            }
        }

        // Static properties for common vectors
        public static Vector4 Zero => new Vector4(0, 0, 0, 0);
        public static Vector4 One => new Vector4(1, 1, 1, 1);

        // Addition and subtraction operators
        public static Vector4 operator +(Vector4 a, Vector4 b) => new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        public static Vector4 operator -(Vector4 a, Vector4 b) => new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);

        // Scalar multiplication and division
        public static Vector4 operator *(Vector4 a, float scalar) => new Vector4(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar);
        public static Vector4 operator /(Vector4 a, float scalar)
        {
            if (scalar == 0) throw new DivideByZeroException("Cannot divide by zero");
            return new Vector4(a.X / scalar, a.Y / scalar, a.Z / scalar, a.W / scalar);
        }

        // Equality operators
        public static bool operator ==(Vector4 a, Vector4 b) => a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
        public static bool operator !=(Vector4 a, Vector4 b) => !(a == b);

        // Dot product
        public static float Dot(Vector4 a, Vector4 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;

        // Override ToString for readable output
        public override string ToString() => $"({X}, {Y}, {Z}, {W})";

        // Override Equals and GetHashCode
        public override bool Equals(object obj) => obj is Vector4 other && this == other;

        // Simple GetHashCode implementation
        public override int GetHashCode() => X.GetHashCode() ^ Y.GetHashCode() ^ Z.GetHashCode() ^ W.GetHashCode();
    }
}
