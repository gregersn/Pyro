#include "pyroshape.h"
#include "pyromath.h"

#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>


typedef Eigen::Matrix<float, 3, 1> Vector3f;

namespace Pyro {

    enum class PointType {
        vertex, curvevertex, beziervertex
    };

    struct t_shapepoint {
        //Pyro::Vector v;
        Vector3f v;
        PointType type;
    };

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

    Shape createshape() { return Shape(); }

    class Shape::impl {
        std::vector<std::vector<Pyro::Vector>> outcontours;
        std::vector<Pyro::Vector> outpoints;
            
        std::vector<std::vector<Pyro::t_shapepoint>> contours;
        std::vector<Pyro::t_shapepoint> points;
        int close;

        Eigen::Transform<float, 3, Eigen::Affine> transform;

    public:
        impl(void) {
            this->transform = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
        }
        ~impl(void) {}
    
        void end(int close) { 
            this->close = close;
            this->outcontours.clear();
            float delta = 1.0 / curve_resolution;

            for(unsigned int contouriterator = 0; contouriterator < this->contours.size(); contouriterator++) {
                auto contour = this->contours[contouriterator];
                this->outpoints.clear();        
                std::cout << "Found contour with " << contour.size() << " points\n";
                for(unsigned int curveiterator = 0; curveiterator < contour.size(); curveiterator++) {
                    auto point = contour[curveiterator];
                    std::cout << "Found point at " << point.v.x() << ", " << point.v.y() << "\n";
                    if(point.type == PointType::curvevertex) {
                        if(curveiterator < 1) {
                            continue;
                        }

                        if((curveiterator + 2) > contour.size()) {
                            continue;
                        }
                        auto p0 = contour[curveiterator - 1];
                        auto p2 = contour[curveiterator + 1];
                        auto p3 = contour[curveiterator + 2];


                        for(unsigned int i = 0; i < curve_resolution; i++) {
                            this->outpoints.push_back(
                                Pyro::Vector(
                                    curvepoint(p0.v.x(), point.v.x(), p2.v.x(), p3.v.x(), i * delta),
                                    curvepoint(p0.v.y(), point.v.y(), p2.v.y(), p3.v.y(), i * delta)
                                )
                            );
                        }
                    } else if(point.type == PointType::beziervertex) {
                        if(curveiterator < 1) {
                            std::cerr << "Missing first vertex\n";
                            continue;
                        }

                        if((curveiterator + 2 ) > contour.size()) {
                            std::cerr << "Missing following vertices\n";
                            continue;
                        }

                        auto p0 = contour[curveiterator - 1];
                        auto p2 = contour[curveiterator + 1];
                        auto p3 = contour[curveiterator + 2];

                        for(unsigned int i = 1; i < curve_resolution + 1; i++) {
                            this->outpoints.push_back(
                                Pyro::Vector(
                                    bezierpoint(p0.v.x(), point.v.x(), p2.v.x(), p3.v.x(), i * delta),
                                    bezierpoint(p0.v.y(), point.v.y(), p2.v.y(), p3.v.y(), i * delta)
                                )
                            );
                        }
                        curveiterator += 2;
                    }
                    else {
                        //Vector3f p = this->transform * point.v.colwise().homogeneous();
                        this->outpoints.push_back(Pyro::Vector(point.v.x(), point.v.y()));
                    }
                }
                this->outcontours.push_back(this->outpoints);
            }
        };

        void vertex(float x, float y) {
            this->points.push_back({Vector3f(x, y, 0.0f), PointType::vertex});
        }

        void curvevertex(float x, float y) {
            this->points.push_back({Vector3f(x, y, 0.0f), PointType::curvevertex});        
        }

        void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) {
            assert(this->points.size() > 0);
            this->points.push_back({Vector3f(x2, y2, 0.0f), PointType::beziervertex});
            this->points.push_back({Vector3f(x3, y3, 0.0f), PointType::beziervertex});
            this->points.push_back({Vector3f(x4, y4, 0.0f), PointType::beziervertex});
        }

        void begin() {
            contours.clear();
            points.clear();      
        }

        void begincontour() {
            if(contours.size() < 1 && points.size() > 0) {
                contours.push_back(points);
                points.clear();
            }
        }

        void endcontour() {
            if(points.size() > 0) {
                contours.push_back(points);
                points.clear();
            }
        }
        std::vector<Pyro::Vector> getpoints() { return this->outpoints; };
        std::vector<std::vector<Pyro::Vector>> getcontours() { return this->outcontours; };

        void rotate(float angle) {
            this->transform.rotate(Eigen::AngleAxisf(angle, Eigen::Vector3f::UnitZ()));
        }

        void nostroke() {

        }

    };

    Shape::Shape(void): pimpl(new impl) {}
    Shape::Shape(const Shape &s): pimpl(new impl(*(s.pimpl))) {}
    Shape::~Shape() { delete this->pimpl; }

    void swap(Shape &s1, Shape &s2) {
        using std::swap;
        swap(s1.pimpl, s2.pimpl);
    }

    Shape &Shape::operator =(Shape s) {
        swap(*(this), s);
        return *(this);
    }

    void Shape::begin() {
        return pimpl->begin();
    }

    void Shape::end(int close) {
        return pimpl->end(close);
    }

    void Shape::begincontour() {
        return pimpl->begincontour();
    }

    void Shape::endcontour() {
        return pimpl->endcontour();
    }

    void Shape::nostroke() {
        return pimpl->nostroke();
    }

    void Shape::rotate(float angle) {
        // return pimpl->rotate(angle);
        this->rotation += angle;
        std::cout << "Rotation: " << this->rotation;
    }

    void Shape::vertex(float x, float y) {
        return pimpl->vertex(x, y);
    }

    void Shape::curvevertex(float x, float y) {
        return pimpl->curvevertex(x, y);
    }

    void Shape::beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) {
        return pimpl->beziervertex(x2, y2, x3, y3, x4, y4);
    }

    std::vector<Pyro::Vector> Shape::getpoints() { return pimpl->getpoints(); };
    std::vector<std::vector<Pyro::Vector>> Shape::getcontours() { return pimpl->getcontours(); };


}
