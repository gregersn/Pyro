#ifndef PYROMATH_H
#define PYROMATH_H

#include <random>
#include <cassert>

namespace Pyro
{

    // *** CALCULATION ***

    template <typename T>
    auto abs(T a) { return std::abs(a); };

    template <typename T>
    auto ceil(T a) { return std::ceil(a); };

    template <typename T>
    T constrain(T value, T low, T high)
    {
        T t{value <= high ? value : high};
        return t >= low ? t : low;
    };

    template <typename T>
    T dist(T x1, T y1, T x2, T y2)
    {
        return mag(x2 - x1, y2 - y1);
    };

    template <typename T>
    T dist(T x1, T y1, T z1, T x2, T y2, T z2)
    {
        return mag(x2 - x1, y2 - y1, z2 - z1);
    };

    float exp(float n);

    template <typename T>
    T lerp(T start, T stop, float amt)
    {
        return (stop * amt) + (start * (1.0 - amt));
    };

    template <typename T>
    T mag(T a, T b)
    {
        return sqrt(a * a + b * b);
    };

    template <typename T>
    T mag(T a, T b, T c)
    {
        return sqrt(a * a + b * b + c * c);
    };

    template <typename T>
    T map(T value, T start1, T stop1, T start2, T stop2)
    {
        return ((value - start1) * (stop2 - start2) / (stop1 - start1)) + start2;
    };

    template <typename T>
    auto floor(T a) { return std::floor(a); };

    template <typename T, typename U>
    auto max(T a, U b) { return a > b ? a : b; };

    template <typename T, typename U>
    auto min(T a, U b) { return a < b ? a : b; };

    float norm(float value, float start, float stop);

    template <typename T, typename U>
    auto pow(T n, U e) { return std::pow(n, e); };

    template <typename T>
    auto round(T a) { return std::round(a); };

    template <typename T>
    auto sq(T a) { return a * a; };

    template <typename T>
    auto sqrt(T a) { return std::sqrt(a); };

    // **** TRIGONOMETRY ***

    template <typename T, typename U>
    auto atan2(T y, U x) { return std::atan2(y, x); };

    template <typename T>
    auto acos(T v) { return std::acos(v); };
    template <typename T>
    auto asin(T v) { return std::asin(v); };
    template <typename T>
    auto atan(T v) { return std::atan(v); };

    float radians(float degree);
    float degrees(float radians);

    template <typename T>
    auto cos(T v) { return std::cos(v); };
    template <typename T>
    auto sin(T v) { return std::sin(v); };
    template <typename T>
    auto tan(T v) { return std::tan(v); };

    // ****** RANDOM *****

    static std::default_random_engine rng{std::default_random_engine(0)};
    static std::uniform_real_distribution<double> rng_dist{std::uniform_real_distribution<double>(0.0, 1.0)};

    // TODO: randomgaussian
    void randomseed(unsigned int seed);

    int random(int range);
    int random(int low, int high);
    double random(); // Returns a random number between 0 and 1
    double random(double range);
    double random(double low, double high);
    unsigned int random(unsigned int range);
    unsigned int random(unsigned int low, unsigned int high);

} // namespace Pyro

#endif
