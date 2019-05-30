#include <pyrovector.h>

#include <cmath>

namespace Pyro {
    Vector::Vector() {
        this->_x = 0.0f;
        this->_y = 0.0f;
        this->_z = 0.0f;
    }

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

    float Vector::dot(Vector const &other) const {
        return _x * other._x + _y * other._y + _z * other._z;
    }

    float Vector::heading() const {
        return atan2(this->_y, this->_x);
    }

    float Vector::angle(Vector const &other) const {
        float dot = this->dot(other);
        float mags = this->mag() * other.mag();
        return acos(dot / mags);
    }

    Vector Vector::div(float v) const{
        return Vector(_x / v, _y / v, _z / v);
    }

    Vector Vector::copy() const {
        return Vector(_x, _y, _z);
    }

    Vector Vector::normalize() const {
        float m = this->mag();
        if(m > 0) {
            return this->div(m);
        }
        return this->copy();
    }
}