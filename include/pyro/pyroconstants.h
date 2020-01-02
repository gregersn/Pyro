#ifndef PYROCONSTANTS_H
#define PYROCONSTANTS_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace Pyro {
    const double PI = M_PI;
    const double TWO_PI = PI * 2.0;
    const double HALF_PI = PI / 2.0;
    const double QUARTER_PI = PI / 4.0;

    const int OPEN = 1;
    const int CLOSE = 2;

    // Arc drawing styles
    const int PIE = 3;
    const int CHORD = 4;
    
    // Stroke caps
    const int ROUND = 5;
    const int SQUARE = 6;
    const int PROJECT = 7;

    const int HSB = 1;
    const int HSBA = 2;
    const int RGB = 3;
    const int RGBA = 4;

}

#endif
