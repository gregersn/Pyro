#ifndef PYROSHAPE_H
#define PYROSHAPE_H

#include <vector>
#include "vector.h"
#include "constants.h"

namespace Pyro
{
    const int DEFAULT{0};
    const int POINTS{1};
    const int LINES{2};
    const int TRIANGLES{3};
    const int TRIANGLE_FAN{4};
    const int TRIANGLE_STRIP{5};
    const int QUADS{6};
    const int QUAD_STRIP{7};

    float bezierpoint(float a, float b, float c, float d, float t);
    float curvepoint(float p0, float p1, float p2, float p3, float t);
    enum class PointType
    {
        VERTEX,
        CURVEVERTEX,
        BEZIERVERTEX
    };

    struct t_shapepoint
    {
        Pyro::Vector v;
        PointType type;
    };

    extern unsigned int curve_resolution;
    void curveresolution(unsigned int res);

    class Shape
    {
        std::vector<std::vector<Pyro::Vector>> outpoints{std::vector<std::vector<Pyro::Vector>>()};
        std::vector<Pyro::t_shapepoint> points{std::vector<Pyro::t_shapepoint>()};
        std::vector<std::vector<Pyro::t_shapepoint>> contours{std::vector<std::vector<Pyro::t_shapepoint>>()};
        int kind{DEFAULT};

    public:
        int close{false};
        Shape();
        ~Shape();
        void begin(int kind = DEFAULT);
        bool is_point_in_path(Vector p);
        bool is_point_in_path(float x, float y);
        void end();
        void end(int close);
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
