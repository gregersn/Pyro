#include <pyroshape.h>
#include <pyromath.h>

namespace Pyro {
    float bezierpoint(float a, float b, float c, float d, float t) {
        float ab = lerp(a, b, t);
        float bc = lerp (b, c, t);
        float cd = lerp(c, d, t);

        float ac = lerp(ab, bc, t);
        float bd = lerp(bc, cd, t);

        return lerp(ac, bd, t);
    }

    Shape::Shape() {
        return;
    }

    Shape::~Shape() {
        return;
    }

    void Shape::vertex(float x, float y) {
        this->points.push_back(Pyro::Vector(x, y));           
    }

    void Shape::beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) {
        assert(this->points.size() > 0);
        float delta = 1.0 / curve_resolution;
        Pyro::Vector prev = this->points[this->points.size() - 1];
        for(unsigned int i = 0; i < curve_resolution; i++) {
            this->points.push_back(
                Pyro::Vector(
                    bezierpoint(prev.x(), x2, x3, x4, i * delta),
                    bezierpoint(prev.y(), y2, y3, y4, i * delta)
                )
            );
        }   
    }
}
