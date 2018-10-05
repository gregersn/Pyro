#include <pyrovector.h>

#include <cmath>

namespace Pyro {
    Vector::Vector(float x, float y) {
        this->_x = x;
        this->_y = y;
    }

    float Vector::x() const {
        return this->_x;
    }

    float Vector::y() const {
        return this->_y;
    }

    float Vector::mag() const {
        return sqrt(_x * _x + _y * _y);
    }

    float Vector::dist(Vector const &other) const {
        return (*this - other).mag();
    }
}