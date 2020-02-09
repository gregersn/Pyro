#include <pyro/pyroshape.h>
#include <pyro/pyromath.h>

#include <iostream>
#include <cassert>

namespace Pyro {
    float bezierpoint(float a, float b, float c, float d, float t) {
        float ab = lerp(a, b, t);
        float bc = lerp (b, c, t);
        float cd = lerp(c, d, t);

        float ac = lerp(ab, bc, t);
        float bd = lerp(bc, cd, t);

        return lerp(ac, bd, t);
    }

    float curvepoint(float p0, float p1, float p2, float p3, float t) {
        return 0.5 * ((2 * p1) +
                     (-p0 + p2) * t +
                     (2 * p0- 5 * p1 + 4 * p2 - p3) * t * t + 
                     (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
    }

    Shape::Shape() {
        return;
    }

    Shape::~Shape() {
        return;
    }

    void Shape::begin() {
         this->points.clear();
         this->outpoints.clear();
    }

    void Shape::end() {
        return this->end(OPEN);
    }

    void Shape::end(int close) {
        this->close = close;
        this->outpoints.clear();        
        float delta = 1.0 / curve_resolution;

        for(unsigned int curveiterator = 0; curveiterator < this->points.size(); curveiterator++) {
            auto point = this->points[curveiterator];
            if(point.type == PointType::curvevertex) {
                if(curveiterator < 1) {
                    continue;
                }

                if((curveiterator + 2) >= this->points.size()) {
                    continue;
                }
                auto p0 = this->points[curveiterator - 1];
                auto p2 = this->points[curveiterator + 1];
                auto p3 = this->points[curveiterator + 2];


                for(unsigned int i = 0; i < curve_resolution + 1; i++) {
                    this->outpoints.push_back(
                        Pyro::Vector(
                            curvepoint(p0.v.x, point.v.x, p2.v.x, p3.v.x, i * delta),
                            curvepoint(p0.v.y, point.v.y, p2.v.y, p3.v.y, i * delta)
                        )
                    );
                }
            } else if(point.type == PointType::beziervertex) {
                if(curveiterator < 1) {
                    std::cerr << "Missing first vertex\n";
                    continue;
                }

                if((curveiterator + 2 ) > this->points.size()) {
                    std::cerr << "Missing following vertices\n";
                    continue;
                }

                auto p0 = this->points[curveiterator - 1];
                auto p2 = this->points[curveiterator + 1];
                auto p3 = this->points[curveiterator + 2];

                for(unsigned int i = 1; i < curve_resolution + 1; i++) {
                    this->outpoints.push_back(
                        Pyro::Vector(
                            bezierpoint(p0.v.x, point.v.x, p2.v.x, p3.v.x, i * delta),
                            bezierpoint(p0.v.y, point.v.y, p2.v.y, p3.v.y, i * delta)
                        )
                    );
                }
                curveiterator += 2;
            }
            else {
                this->outpoints.push_back(point.v);
            }
        }
        
    };

    void Shape::vertex(float x, float y) {
        this->points.push_back({Pyro::Vector(x, y), PointType::vertex});
    }

    void Shape::curvevertex(float x, float y) {
        this->points.push_back({Pyro::Vector(x, y), PointType::curvevertex});        
    }

    void Shape::beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) {
        assert(this->points.size() > 0);
        this->points.push_back({Pyro::Vector(x2, y2), PointType::beziervertex});
        this->points.push_back({Pyro::Vector(x3, y3), PointType::beziervertex});
        this->points.push_back({Pyro::Vector(x4, y4), PointType::beziervertex});
    }

}
