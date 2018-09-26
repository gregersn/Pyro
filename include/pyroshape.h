#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include <pyrovector.h>
#include <pyroconstants.h>

namespace Pyro {
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
            std::vector<Pyro::Vector> getpoints() { return this->points; };
    };
};
#endif