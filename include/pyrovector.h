#ifndef PYROVECTOR_H
#define PYROVECTOR_H

namespace Pyro {
    class Vector {
        private:
        float _x;
        float _y;
        float _z;

        public:
            float x() const;
            float y() const;
            float z() const;
            
            Vector();
            Vector(float x, float y);
            Vector(float x, float y, float z);
    
            float dist(const Vector &other) const;
            float mag() const;
            float dot(const Vector &other) const;
            float heading() const;
            float angle(Vector const &other) const;

            inline Vector operator+ (Vector const &other) const {
                return Vector(_x + other._x, _y + other._y, _z + other._z);
            };

            inline Vector operator- (Vector const &other) const {
                return Vector(_x - other._x, _y - other._y, _z - other._z);
            }

            friend inline bool operator==(const Vector &lhs, const Vector &rhs) {
                return lhs.dist(rhs) < __DBL_EPSILON__;
            }

            void operator=(const Vector &rhs) {
                this->_x = rhs.x();
                this->_y = rhs.y();
                this->_z = rhs.z();
            }

    };
}

#endif