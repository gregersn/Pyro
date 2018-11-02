#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include "pyrovector.h"
#include "pyroconstants.h"

namespace Pyro {
    const unsigned int curve_resolution = 32;
    class Shape {
            std::vector<Pyro::Vector> points;
        public:
            int close;
            Shape();
            ~Shape();
            void begin() { points.clear(); };
            void end() { this->end(OPEN); };
            void end(int close) { this->close = close; };
            void vertex(float x, float y);
            void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4);
            std::vector<Pyro::Vector> getpoints() { return this->points; };
    };
};
#endif