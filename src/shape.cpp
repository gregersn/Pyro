#include "pyro/shape.h"
#include "pyro/constants.h"

#include "earcut.hpp"

#include <iostream>
#include <cassert>
#include <vector>

namespace mapbox
{
    namespace util
    {
        template <>
        struct nth<0, Pyro::Vector>
        {
            inline static auto get(const Pyro::Vector &t)
            {
                return t.x;
            };
        };
        template <>
        struct nth<1, Pyro::Vector>
        {
            inline static auto get(const Pyro::Vector &t)
            {
                return t.y;
            };
        };
    } // namespace util

} // namespace mapbox
namespace Pyro
{
    unsigned int curve_resolution = 32;

    void curveresolution(unsigned int res) { curve_resolution = res; }

    void splineknots(std::vector<int> *knots, unsigned int point_count, unsigned int degree, bool clamp)
    {
        knots->push_back(0);
        unsigned int knotcount = (point_count) + (degree) + 1;

        if (clamp)
        {
            knotcount -= (degree);
            for (unsigned int knot = 1; knot < degree; knot++)
            {
                knots->push_back(0);
            }
            knotcount -= (degree - 1);
        }

        for (unsigned int knot = 1; knot < knotcount; knot++)
        {
            knots->push_back(knot);
        }

        if (clamp)
        {
            for (unsigned int knot = 0; knot < degree; knot++)
            {
                knots->push_back(point_count - 1);
            }
        }
    }

    template <typename T>
    T bsplinepoint(const std::vector<T> &points, float t, unsigned int degree, bool clamp, std::vector<int> &knots)
    {
        if (degree < 1)
        {
            return points[0];
        }

        unsigned int point_count = points.size();

        if (degree > (point_count - 1))
        {
            return points[0];
        }

        // Create weight vector, same length as points.
        std::vector<float> weights;
        for (unsigned int i = 0; i < point_count; i++)
        {
            weights.push_back(1.0f);
        }

        assert(weights.size() == point_count);

        if (knots.size() == 0)
        {
            splineknots(&knots, point_count, degree, clamp);
        }

        assert(knots.size() == (point_count + degree + 1));

        int domain_bottom = degree;
        int domain_top = knots.size() - 1 - degree;

        int low = knots[domain_bottom];
        int high = knots[domain_top];

        t = t * (high - low) + low;

        if (t < low)
            t = low;
        // return points[0];

        if (t > high)
            t = high;
        // return points[domain_top - 2];

        int s = 0;
        for (s = domain_bottom; s < domain_top; s++)
        {
            if (t >= knots[s] && t <= knots[s + 1])
            {
                break;
            }
        }

        // convert points to homogeneous coordinates
        std::vector<T> v;
        for (unsigned int i = 0; i < point_count; i++)
        {
            v.push_back(points[i] * weights[i]);
        }

        float alpha = 0.0f;
        for (unsigned int l = 1; l <= degree + 1; l++)
        {
            for (unsigned int i = s; i > s - degree - 1 + l; i--)
            {
                alpha = (t - knots[i]) / (knots[i + degree + 1 - l] - knots[i]);
                v[i] = (1 - alpha) * v[i - 1] + alpha * v[i];
            }
        }

        return v[s] / weights[s];
    }

    template Vector bsplinepoint(const std::vector<Vector> &points, float t, unsigned int degree, bool clamp, std::vector<int> &knots);

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

    std::vector<int32_t> Shape::indices()
    {
        return mapbox::earcut<int32_t>(this->getpoints());
    }

} // namespace Pyro
