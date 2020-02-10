#ifndef PYROVECTOR_H
#define PYROVECTOR_H

namespace Pyro {
    class Vector {
        public:
            float x;
            float y;
            float z;

            Vector();
            Vector(float x, float y);
            Vector(float x, float y, float z);
    
            float dist(const Vector &other) const;
            float mag() const;
            float dot(const Vector &other) const;
            float heading() const;
            float angle(Vector const &other) const;
            Vector rotate(const float a) const;

            Vector normalize() const;
            Vector copy() const;
            Vector div(float v) const;
            Vector setmag(float mag) const;
            Vector limit(float mag) const;

            static Vector fromangle(float a);
            static Vector random2d();
            static Vector random3d();

            inline Vector operator+ (Vector const &other) const {
                return Vector(x + other.x, y + other.y, z + other.z);
            };

            inline void operator+= (Vector const &other) {
                x += other.x;
                y += other.y;
                z += other.z;
            };

            inline Vector operator- (Vector const &other) const {
                return Vector(x - other.x, y - other.y, z - other.z);
            }

            inline Vector operator/ (float const v) const {
                return Vector(x / v, y / v, z / v);
            }

            inline Vector operator* (float const v) const {
                return Vector(x * v, y * v, z * v);
            }


            friend inline bool operator==(const Vector &lhs, const Vector &rhs) {
                return lhs.dist(rhs) < __DBL_EPSILON__;
            }

            void operator=(const Vector &rhs) {
                this->x = rhs.x;
                this->y = rhs.y;
                this->z = rhs.z;
            }

    };
}

#endif