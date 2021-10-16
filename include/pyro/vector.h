#ifndef PYROVECTOR_H
#define PYROVECTOR_H

#include <string>
#include <sstream>

namespace Pyro
{
    class Vector
    {
    public:
        float x;
        float y;
        float z;

        Vector();
        Vector(float x, float y, float z = 0.0f);

        static Vector random2d();
        static Vector random3d();

        static Vector fromangle(float a);

        Vector copy() const;

        float mag() const;
        float magsq() const;

        inline Vector operator+(Vector const &other) const
        {
            return Vector(x + other.x, y + other.y, z + other.z);
        };

        inline void operator+=(Vector const &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        };

        inline Vector operator-(Vector const &other) const
        {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        inline void operator-=(Vector const &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        };

        inline Vector operator/(float const v) const
        {
            return Vector(x / v, y / v, z / v);
        }

        inline void operator/=(float const v)
        {
            x /= v;
            y /= v;
            z /= v;
        };

        inline Vector operator*(float const v) const
        {
            return Vector(x * v, y * v, z * v);
        }

        inline void operator*=(float const v)
        {
            x *= v;
            y *= v;
            z *= v;
        };

        friend inline bool operator==(const Vector &lhs, const Vector &rhs)
        {
            return lhs.dist(rhs) < __DBL_EPSILON__;
        }

        Vector &operator=(const Vector &rhs)
        {
            this->x = rhs.x;
            this->y = rhs.y;
            this->z = rhs.z;
            return *this;
        }

        float dist(const Vector &other) const;
        float dot(const Vector &other) const;
        Vector cross(const Vector &other) const;

        Vector normalize() const;
        Vector limit(float mag) const;
        Vector setmag(float mag) const;

        float heading() const;

        Vector rotate(const float a) const;

        float angle(Vector const &other) const;

        Vector div(float v) const;

        std::string tostring()
        {
            std::ostringstream oss;
            oss << "[" << x << ", " << y << ", " << z << "]";
            return oss.str();
        }
    };
}

#endif