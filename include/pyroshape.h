#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include "pyrovector.h"
#include "pyroconstants.h"

namespace Pyro {
    enum class PointType {
        vertex, curvevertex, beziervertex
    };

    struct t_shapepoint {
        Pyro::Vector v;
        PointType type;
    };

    const unsigned int curve_resolution = 32;

    class Shape {
            std::vector<std::vector<Pyro::Vector>> outcontours;
            std::vector<Pyro::Vector> outpoints;
            
            std::vector<std::vector<Pyro::t_shapepoint>> contours;
            std::vector<Pyro::t_shapepoint> points;

        public:
            int close;
            Shape();
            ~Shape();
            
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
            std::vector<Pyro::Vector> getpoints() { return this->outpoints; };
            std::vector<std::vector<Pyro::Vector>> getcontours() { return this->outcontours; };
    };

    Shape createshape();
};
#endif