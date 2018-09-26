#ifndef PYROVECTOR_H
#define PYROVECTOR_H

namespace Pyro {
    class Vector {
        private:
        float _x;
        float _y;

        public:
            float x() const;
            float y() const;
            Vector(float x, float y);
            float dist(const Vector &other) const;
            float mag() const;

            inline Vector operator+ (Vector const &other) const {
                return Vector(_x + other._x, _y + other._y);
            };

            inline Vector operator- (Vector const &other) const {
                return Vector(_x - other._x, _y - other._y);
            }

            friend inline bool operator==(const Vector &lhs, const Vector &rhs) {
                return lhs.dist(rhs) < __DBL_EPSILON__;
            }
    };
}

#endif