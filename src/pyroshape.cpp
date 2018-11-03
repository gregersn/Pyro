#include <pyroshape.h>
#include <pyromath.h>

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

    void Shape::end(int close) { 
        this->close = close;
        this->outpoints.clear();        
        for(unsigned int i = 0; i < this->points.size(); i++) {
            auto point = this->points[i];
            if(point.type == PointType::curvevertex) {
                if(i < 1) {
                    continue;
                }

                if((i + 2) > this->points.size()) {
                    continue;
                }
                auto p0 = this->points[i - 1];
                auto p2 = this->points[i + 1];
                auto p3 = this->points[i + 2];

                float delta = 1.0 / curve_resolution;

                for(unsigned int i = 0; i < curve_resolution; i++) {
                    this->outpoints.push_back(
                        Pyro::Vector(
                            curvepoint(p0.v.x(), point.v.x(), p2.v.x(), p3.v.x(), i * delta),
                            curvepoint(p0.v.y(), point.v.y(), p2.v.y(), p3.v.y(), i * delta)
                        )
                    );
                }
            } else {
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
        float delta = 1.0 / curve_resolution;
        t_shapepoint prev = this->points[this->points.size() - 1];
        for(unsigned int i = 0; i < curve_resolution + 1; i++) {
            this->points.push_back({
                Pyro::Vector(
                    bezierpoint(prev.v.x(), x2, x3, x4, i * delta),
                    bezierpoint(prev.v.y(), y2, y3, y4, i * delta)
                ), PointType::vertex}
            );
        }   
    }

}
