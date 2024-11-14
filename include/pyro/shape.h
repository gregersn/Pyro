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

    template <typename T>
    T bezierpoint(T a, T b, T c, T d, float t);

    template <typename T>
    T curvepoint(T p0, T p1, T p2, T p3, float t);

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
        void vertex(Vector p);
        void vertex(float x, float y) { this->vertex(Vector(x, y)); };
        void curvevertex(Vector p);
        void curvevertex(float x, float y) { this->curvevertex(Vector(x, y)); };
        void beziervertex(Vector p2, Vector p3, Vector p4);
        void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) { this->beziervertex(Vector(x2, y2), Vector(x3, y3), Vector(x4, y4)); };
        std::vector<std::vector<Pyro::Vector>> getpoints() { return this->outpoints; };
    };
};
#endif
