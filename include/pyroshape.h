#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include <experimental/propagate_const>
#include "pyrovector.h"
#include "pyroconstants.h"

namespace Pyro {
    const unsigned int curve_resolution = 32;

    class Shape {
        private:
            class impl;
            impl *pimpl;

        public:
            int close;
            Shape(void);
            Shape(const Shape & s);
            ~Shape(void);

            Shape & operator =(Shape s);

            friend void swap(Shape &s1, Shape &s2);

            void begin();
            inline void end() { this->end(false); };
            void end(int close);

            void begincontour();
            void endcontour();

            void nostroke();
            void rotate(float angle);

            void vertex(float x, float y);
            void curvevertex(float x, float y);
            void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4);
            std::vector<Pyro::Vector> getpoints();
            std::vector<std::vector<Pyro::Vector>> getcontours();
    };

    Shape createshape();
};
#endif