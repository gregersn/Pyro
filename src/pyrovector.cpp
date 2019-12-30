#include <pyro/pyrovector.h>
#include <pyro/pyromath.h>
#include <cmath>

namespace Pyro {
    Vector::Vector() {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vector::Vector(float x, float y) {
        this->x = x;
        this->y = y;
        this->z = 0.0f;
    }

    Vector::Vector(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector Vector::fromangle(float a) {
        return Vector(cos(a), sin(a));
    }

    Vector Vector::random2d() {
        return Vector::fromangle(Pyro::random(1.0f) * M_2_PI);
    }

    Vector Vector::random3d() {
        float phi = Pyro::random(0.0f, 1.0f * M_2_PI);
        float costheta = Pyro::random(-1.0f, 1.0f);

        float theta = acos(costheta);
        return Vector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    }

    float Vector::mag() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector Vector::setmag(float m) const {
        return this->normalize() * m;
    }

    float Vector::dist(Vector const &other) const {
        return (*this - other).mag();
    }

    float Vector::dot(Vector const &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float Vector::heading() const {
        return atan2(this->y, this->x);
    }

    float Vector::angle(Vector const &other) const {
        float dot = this->dot(other);
        float mags = this->mag() * other.mag();
        return acos(dot / mags);
    }

    Vector Vector::div(float v) const{
        return Vector(x / v, y / v, z / v);
    }

    Vector Vector::copy() const {
        return Vector(x, y, z);
    }

    Vector Vector::normalize() const {
        float m = this->mag();
        if(m > 0) {
            return this->div(m);
        }
        return this->copy();
    }

    Vector Vector::limit(float m) const {
        if(this->mag() > m) {
            return this->setmag(m);
        }
        return this->copy();
    }
}