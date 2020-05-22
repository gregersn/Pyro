#include <pyro/pyrovector.h>
#include <pyro/pyromath.h>
#include <cmath>

namespace Pyro {
    Vector::Vector() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector::Vector(float x, float y) : x(x), y(y), z(0.0f) {}
    Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector Vector::random2d() {
        return Vector::fromangle(Pyro::random(1.0f) * M_2_PI);
    }

    Vector Vector::random3d() {
        float phi = Pyro::random(0.0f, 1.0f * M_2_PI);
        float costheta = Pyro::random(-1.0f, 1.0f);

        float theta = acos(costheta);
        return Vector(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    }

    Vector Vector::fromangle(float a) {
        return Vector(cos(a), sin(a));
    }

    Vector Vector::copy() const {
        return Vector(x, y, z);
    }

    float Vector::mag() const {
        return sqrt(x * x + y * y + z * z);
    }

    float Vector::magsq() const {
        return x * x + y * y + z * z;
    }

    float Vector::dist(Vector const &other) const {
        return (*this - other).mag();
    }

    float Vector::dot(Vector const &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector Vector::cross(Vector const &other) const {
        float crossx = y * other.z - other.y * z;
        float crossy = z * other.x - other.z * x;
        float crossz = x * other.y - other.x * y;

        return Vector(crossx, crossy, crossz);
    }

    Vector Vector::normalize() const {
        float m = this->mag();
        if(m > 0) {
            return this->div(m);
        }
        return this->copy();
    }

    Vector Vector::limit(float m) const {
        if(this->magsq() > m * m) {
            return this->setmag(m);
        }
        return this->copy();
    }

    Vector Vector::setmag(float m) const {
        return this->normalize() * m;
    }

    float Vector::heading() const {
        return atan2(this->y, this->x);
    }

    Vector Vector::rotate(const float a) const {
        return Vector(x * cos(a) - y * sin(a),
                      x * sin(a) + y * cos(a));
    }

    float Vector::angle(Vector const &other) const {
        if(x == 0 && y == 0 && z == 0) return 0.0f;
        if(other.x == 0 && other.y == 0 && other.z == 0) return 0.0f;
        
        float dot = this->dot(other);
        float mags = this->mag() * other.mag();
        return acos(dot / mags);
    }

    Vector Vector::div(float v) const{
        return Vector(x / v, y / v, z / v);
    }

}