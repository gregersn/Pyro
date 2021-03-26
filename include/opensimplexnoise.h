/*
 * OpenSimplex Noise in Java.
 * by Kurt Spencer
 * Converted to C++ in 2020
 * by Greger Stolt Nilsen
 * 
 * 
 * v1.1 (October 5, 2014)
 * - Added 2D and 4D implementations.
 * - Proper gradient sets for all dimensions, from a
 *   dimensionally-generalizable scheme with an actual
 *   rhyme and reason behind it.
 * - Removed default permutation array in favor of
 *   default seed.
 * - Changed seed-based constructor to be independent
 *   of any particular randomization library, so results
 *   will be the same when ported to other languages.
 * 
 * This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Source: https://gist.github.com/KdotJPG/b1270127455a94ac5d19
 */

#ifndef OPENSIMPLEXNOISE_H
#define OPENSIMPLEXNOISE_H

#include <array>
#include <cstdint>

class OpenSimplexNoise
{
private:
    static const double constexpr STRETCH_CONSTANT_2D = -0.211324865405187; //(1/Math.sqrt(2+1)-1)/2;
    static const double constexpr SQUISH_CONSTANT_2D = 0.366025403784439;   //(Math.sqrt(2+1)-1)/2;
    static const double constexpr STRETCH_CONSTANT_3D = -1.0 / 6.0;         //(1/Math.sqrt(3+1)-1)/3;
    static const double constexpr SQUISH_CONSTANT_3D = 1.0 / 3.0;           //(Math.sqrt(3+1)-1)/3;
    static const double constexpr STRETCH_CONSTANT_4D = -0.138196601125011; //(1/Math.sqrt(4+1)-1)/4;
    static const double constexpr SQUISH_CONSTANT_4D = 0.309016994374947;   //(Math.sqrt(4+1)-1)/4;

    static const double constexpr NORM_CONSTANT_2D = 47;
    static const double constexpr NORM_CONSTANT_3D = 103;
    static const double constexpr NORM_CONSTANT_4D = 30;

    static const long DEFAULT_SEED = 0;

    std::array<int16_t, 256> permGradIndex3D;
    std::array<int16_t, 256> perm;

    double extrapolate(int xsb, int ysb, double dx, double dy);
    double extrapolate(int xsb, int ysb, int zsb, double dx, double dy, double dz);
    double extrapolate(int xsb, int ysb, int zsb, int wsb, double dx, double dy, double dz, double dw);
    static int fastFloor(double x);
    static const std::array<int8_t, 16> gradients2D;
    static const std::array<int8_t, 72> gradients3D;
    static const std::array<int8_t, 256> gradients4D;

public:
    OpenSimplexNoise() : OpenSimplexNoise(DEFAULT_SEED){};
    OpenSimplexNoise(std::array<int16_t, 256> perm);
    OpenSimplexNoise(long seed);
    ~OpenSimplexNoise();
    double eval(double x, double y);
    double eval(double x, double y, double z);
    double eval(double x, double y, double z, double w);
};
#endif