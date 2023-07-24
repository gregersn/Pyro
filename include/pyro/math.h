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

    // TODO: dist
    // TODO: exp

    template <typename T>
    T lerp(T start, T stop, float amt)
    {
        return (stop * amt) + (start * (1.0 - amt));
    };

    // TODO: log
    // TODO: mag

    template <typename T>
    T map(T value, T start1, T stop1, T start2, T stop2)
    {
        return ((value - start1) * (stop2 - start2) / (stop1 - start1)) + start2;
    };

    // TODO: floor

    template <typename T, typename U>
    auto max(T a, U b) { return a > b ? a : b; };

    template <typename T, typename U>
    auto min(T a, U b) { return a < b ? a : b; };

    // TODO: norm
    // TODO: pow

    // TODO: round
    // TODO: sq
    // TODO: sqrt

    // **** TRIGONOMETRY ***

    /* TODO:
    acos
    asin
    atan2
    atan
    cos
    */
    float radians(float degree);
    float degrees(float radians);

    /*  TODO:
    sin
    tan
    */

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
