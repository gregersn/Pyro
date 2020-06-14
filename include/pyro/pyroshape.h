#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include "pyrovector.h"
#include "pyroconstants.h"

namespace Pyro {
    float bezierpoint(float a, float b, float c, float d, float t);
    float curvepoint(float p0, float p1, float p2, float p3, float t);
    enum class PointType {
        vertex, curvevertex, beziervertex
    };

    struct t_shapepoint {
        Pyro::Vector v;
        PointType type;
    };

    const unsigned int curve_resolution = 32;

    class Shape {
            std::vector<std::vector<Pyro::Vector>> outpoints = std::vector<std::vector<Pyro::Vector>>();
            std::vector<Pyro::t_shapepoint> points = std::vector<Pyro::t_shapepoint>();
            std::vector<std::vector<Pyro::t_shapepoint>> contours = std::vector<std::vector<Pyro::t_shapepoint>>();
        public:
            int close = false;
            Shape();
            ~Shape();
            void begin();
            void begin(int kind);
            void end();
            void end(int mode);
            void begincontour();
            void endcontour();
            void vertex(Vector v) { this->vertex(v.x, v.y); };
            void vertex(float x, float y);
            void curvevertex(float x, float y);
            void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4);
            std::vector<std::vector<Pyro::Vector>> getpoints() { return this->outpoints; };
    };
};
#endif