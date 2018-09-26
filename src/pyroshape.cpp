#include <pyroshape.h>

namespace Pyro {
    Shape::Shape() {
        return;
    }

    Shape::~Shape() {
        return;
    }

    void Shape::vertex(float x, float y) {
        this->points.push_back(Pyro::Vector(x, y));           
    }
}
