#ifndef PYROVECTOR_H
#define PYROVECTOR_H

#include <string>
#include <sstream>

namespace Pyro
{
    class Vector
    {
    public:
        float x{};
        float y{};
        float z{};

        Vector();
        Vector(Vector const &other);
        Vector(float x, float y, float z = 0.0f);

        static Vector random2d();
        static Vector random3d();

        static Vector fromangle(float a);

        Vector copy() const;

        float mag() const;
        float magsq() const;

        Vector operator+(Vector const &other) const
        {
            return Vector(x + other.x, y + other.y, z + other.z);
        };

        void operator+=(Vector const &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
        };

        Vector operator-(Vector const &other) const
        {
            return Vector(x - other.x, y - other.y, z - other.z);
        }

        void operator-=(Vector const &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        };

        Vector operator/(float const v) const
        {
            return Vector(x / v, y / v, z / v);
        }

        void operator/=(float const v)
        {
            x /= v;
            y /= v;
            z /= v;
        };

        Vector operator*(float const v) const
        {
            return Vector(x * v, y * v, z * v);
        }

        void operator*=(float const v)
        {
            x *= v;
            y *= v;
            z *= v;
        };

        // The comparison operators are implemented as member functions, to be compatible with SIP.
        // Based on information found here: https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading/4421715
        bool operator==(const Vector &rhs) const
        {
            return this->dist(rhs) < __DBL_EPSILON__;
        }

        bool operator<(const Vector &rhs) const
        {
            return (this->x < rhs.x - __DBL_EPSILON__ || (std::abs(this->x - rhs.x) < __DBL_EPSILON__ and this->y < rhs.y - __DBL_EPSILON__));
        }

        bool operator!=(const Vector &rhs) const
        {
            return !this->operator==(rhs);
        }

        bool operator>(const Vector &rhs) const
        {
            return rhs.operator<(*this);
        }

        bool operator<=(const Vector &rhs) const
        {
            return !this->operator>(rhs);
        }

        bool operator>=(const Vector &rhs) const
        {
            return !this->operator<(rhs);
        }

        // Removed 2024-04-01, not needed? Gives a lot of warnings.
        /*Vector &operator=(const Vector &rhs)
        {
            this->x = rhs.x;
            this->y = rhs.y;
            this->z = rhs.z;
            return *this;
        }*/

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
