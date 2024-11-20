#include "pyro/vector.h"
#include "pyro/transformer.h"
#include <iostream>

namespace Pyro
{
    Transformer2D::Transformer2D()
    {
        this->current = Eigen::Affine2d::Identity();
        this->stack = std::vector<Eigen::Affine2d>();
    }
    void Transformer2D::translate(Vector v)
    {
        Eigen::Vector2d t = Eigen::Vector2d(v.x, v.y);
        this->current *= Eigen::Translation2d(t);
    }

    void Transformer2D::translate(float x, float y)
    {
        Eigen::Vector2d t = Eigen::Vector2d(x, y);
        this->current *= Eigen::Translation2d(t);
    }

    void Transformer2D::rotate(float a)
    {
        Eigen::Rotation2D<double> r = Eigen::Rotation2D<double>(a);
        this->current *= r;
    }

    void Transformer2D::scale(float sx, float sy)
    {
        this->current *= Eigen::Scaling<double>(sx, sy);
    }

    void Transformer2D::scale(Vector v)
    {
        this->current *= Eigen::Scaling<double>(v.x, v.y);
    }

    void Transformer2D::pushmatrix()
    {
        this->stack.push_back(Eigen::Affine2d(this->current));
    }

    void Transformer2D::popmatrix()
    {
        this->current = this->stack[this->stack.size() - 1];
        this->stack.pop_back();
    }

    Pyro::Vector Transformer2D::apply(Pyro::Vector v)
    {
        Eigen::Vector2d out = this->current * Eigen::Vector2d(v.x, v.y);
        return Pyro::Vector(out(0), out(1));
    }
} // namespace Pyro
