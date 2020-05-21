#ifndef PYRO_TRANSFORMER_H
#define PYRO_TRANSFORMER_H

#include <pyro/pyrovector.h>
#include <eigen3/Eigen/Geometry>

namespace Pyro {
    class Transformer2D {
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
    };
}
#endif
