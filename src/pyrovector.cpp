#include <pyrovector.h>

#include <cmath>

namespace Pyro {
    Vector::Vector(float x, float y) {
        this->_x = x;
        this->_y = y;
        this->_z = 0.0f;
    }

    Vector::Vector(float x, float y, float z) {
        this->_x = x;
        this->_y = y;
        this->_z = z;
    }

    float Vector::x() const {
        return this->_x;
    }

    float Vector::y() const {
        return this->_y;
    }

    float Vector::z() const {
        return this->_z;
    }

    float Vector::mag() const {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }

    float Vector::dist(Vector const &other) const {
        return (*this - other).mag();
    }
}