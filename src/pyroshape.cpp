#include <pyro/pyroshape.h>
#include <pyro/pyromath.h>

#include <iostream>
#include <cassert>

namespace Pyro
{
    unsigned int curve_resolution = 32;
    void curveresolution(unsigned int res) { curve_resolution = res; }
    float bezierpoint(float a, float b, float c, float d, float t)
    {
        float ab = lerp(a, b, t);
        float bc = lerp(b, c, t);
        float cd = lerp(c, d, t);

        float ac = lerp(ab, bc, t);
        float bd = lerp(bc, cd, t);

        return lerp(ac, bd, t);
    }

    float curvepoint(float p0, float p1, float p2, float p3, float t)
    {
        return 0.5 * ((2 * p1) +
                      (-p0 + p2) * t +
                      (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
                      (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
    }

    bool Shape::is_point_in_path(float x, float y)
    {
        uint num = this->points.size();
        uint j = num - 1;
        bool c = false;
        for (uint i = 0; i < num; i++)
        {
            if (((this->points[i].v.y > y) != (this->points[j].v.y > y)) &&
                (x < this->points[i].v.x + (this->points[j].v.x - this->points[i].v.x) * (y - this->points[i].v.y) /
                                               (this->points[j].v.y - this->points[i].v.y)))
                c = !c;
            j = i;
        }
        return c;
    }

    Shape::Shape()
    {
        return;
    }

    Shape::~Shape()
    {
        return;
    }

    void Shape::begin()
    {
        this->contours.clear();
        this->points.clear();
        this->outpoints.clear();
    }

    void Shape::end()
    {
        return this->end(OPEN);
    }

    void Shape::end(int close)
    {
        this->contours.push_back(this->points);
        this->close = close;
        this->outpoints.clear();
        float delta = 1.0 / curve_resolution;

        for (size_t contour_iterator = 0; contour_iterator < this->contours.size(); contour_iterator++)
        {
            std::vector<t_shapepoint> points = this->contours[contour_iterator];
            std::vector<Pyro::Vector> contour = std::vector<Pyro::Vector>();
            for (unsigned int curveiterator = 0; curveiterator < points.size(); curveiterator++)
            {
                auto point = points[curveiterator];
                if (point.type == PointType::curvevertex)
                {
                    if (curveiterator < 1)
                    {
                        continue;
                    }

                    if ((curveiterator + 2) >= points.size())
                    {
                        continue;
                    }
                    auto p0 = points[curveiterator - 1];
                    auto p2 = points[curveiterator + 1];
                    auto p3 = points[curveiterator + 2];

                    for (unsigned int i = 0; i < curve_resolution + 1; i++)
                    {
                        contour.push_back(
                            Pyro::Vector(
                                curvepoint(p0.v.x, point.v.x, p2.v.x, p3.v.x, i * delta),
                                curvepoint(p0.v.y, point.v.y, p2.v.y, p3.v.y, i * delta)));
                    }
                }
                else if (point.type == PointType::beziervertex)
                {
                    if (curveiterator < 1)
                    {
                        std::cerr << "Missing first vertex\n";
                        continue;
                    }

                    if ((curveiterator + 2) > points.size())
                    {
                        std::cerr << "Missing following vertices\n";
                        continue;
                    }

                    auto p0 = points[curveiterator - 1];
                    auto p2 = points[curveiterator + 1];
                    auto p3 = points[curveiterator + 2];

                    for (unsigned int i = 1; i < curve_resolution + 1; i++)
                    {
                        contour.push_back(
                            Pyro::Vector(
                                bezierpoint(p0.v.x, point.v.x, p2.v.x, p3.v.x, i * delta),
                                bezierpoint(p0.v.y, point.v.y, p2.v.y, p3.v.y, i * delta)));
                    }
                    curveiterator += 2;
                }
                else
                {
                    contour.push_back(point.v);
                }
            }

            this->outpoints.push_back(contour);
        }
    }

    void Shape::begincontour()
    {
        if (this->points.size() > 0)
        {
            this->contours.push_back(this->points);
        }
        this->points = std::vector<Pyro::t_shapepoint>();
    }

    void Shape::endcontour()
    {
    }

    void Shape::vertex(float x, float y)
    {
        this->points.push_back({Pyro::Vector(x, y), PointType::vertex});
    }

    void Shape::curvevertex(float x, float y)
    {
        this->points.push_back({Pyro::Vector(x, y), PointType::curvevertex});
    }

    void Shape::beziervertex(float x2, float y2, float x3, float y3, float x4, float y4)
    {
        assert(this->points.size() > 0);
        this->points.push_back({Pyro::Vector(x2, y2), PointType::beziervertex});
        this->points.push_back({Pyro::Vector(x3, y3), PointType::beziervertex});
        this->points.push_back({Pyro::Vector(x4, y4), PointType::beziervertex});
    }

}
