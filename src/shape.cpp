#include "pyro/shape.h"
#include "pyro/math.h"

#include <iostream>
#include <cassert>

namespace Pyro
{
    unsigned int curve_resolution = 32;
    void curveresolution(unsigned int res) { curve_resolution = res; }


    Shape::Shape()
    {
        return;
    }

    Shape::~Shape()
    {
        return;
    }

    void Shape::begin(int kind)
    {
        this->kind = kind;
        this->contours.clear();
        this->points.clear();
        this->outpoints.clear();
    }

    void Shape::end()
    {
        return this->end(OPEN);
    }

    bool Shape::is_point_in_path(Vector p)
    {
        return this->is_point_in_path(p.x, p.y);
    }

    bool Shape::is_point_in_path(float x, float y)
    {
        int num = this->points.size();
        int j = num - 1;
        bool c = false;
        for (int i = 0; i < num; i++)
        {
            if (((this->points[i].v.y > y) != (this->points[j].v.y > y)) &&
                (x < this->points[i].v.x + (this->points[j].v.x - this->points[i].v.x) * (y - this->points[i].v.y) /
                                               (this->points[j].v.y - this->points[i].v.y)))
                c = !c;
            j = i;
        }
        return c;
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
                if (point.type == PointType::CURVEVERTEX)
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
                        contour.push_back(curvepoint(p0.v, point.v, p2.v, p3.v, i * delta));
                    }
                }
                else if (point.type == PointType::BEZIERVERTEX)
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
                        contour.push_back(bezierpoint(p0.v, point.v, p2.v, p3.v, i * delta));
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

    void Shape::vertex(Vector p)
    {
        this->points.push_back({p, PointType::VERTEX});
    }

    void Shape::curvevertex(Vector p)
    {
        this->points.push_back({p, PointType::CURVEVERTEX});
    }

    void Shape::beziervertex(Vector p2, Vector p3, Vector p4)
    {
        assert(this->points.size() > 0);
        this->points.push_back({p2, PointType::BEZIERVERTEX});
        this->points.push_back({p3, PointType::BEZIERVERTEX});
        this->points.push_back({p4, PointType::BEZIERVERTEX});
    }

}
