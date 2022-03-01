#ifndef PYRO_TRANSFORMER_H
#define PYRO_TRANSFORMER_H

#include <vector>
#include "vector.h"
#include <eigen3/Eigen/Geometry>

namespace Pyro
{
    class Transformer2D
    {
    private:
        std::vector<Eigen::Affine2d> stack;
        Eigen::Affine2d current;

    public:
        Transformer2D();
        void translate(float x, float y);
        void rotate(float a);
        void scale(float sx, float sy);

        void pushmatrix();
        void popmatrix();

        Pyro::Vector apply(Pyro::Vector v);

        Eigen::Affine2d getcurrent() { return this->current; }

        Pyro::Vector screen(Pyro::Vector v) { return this->apply(v); };

        float screen_x(Pyro::Vector v) { return this->apply(v).x; };
        float screen_x(float x, float y, float z = 0.0f) { return this->screen_x(Vector(x, y, z)); };
        float screen_y(Pyro::Vector v) { return this->apply(v).y; };
        float screen_y(float x, float y, float z = 0.0f) { return this->screen_y(Vector(x, y, z)); };
    };

}
#endif
