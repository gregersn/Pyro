#ifndef PYROCONSTANTS_H
#define PYROCONSTANTS_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace Pyro
{
    const double PI{M_PI};
    const double TWO_PI{PI * 2.0};
    const double HALF_PI{PI / 2.0};
    const double QUARTER_PI{PI / 4.0};

    const int OPEN{1};
    const int CLOSE{2};

    // Arc drawing styles
    const int PIE{3};
    const int CHORD{4};

    // Shape drawing modes (rect, ellipse, image, shape)
    const int CORNER{0};
    const int CORNERS{1};
    const int CENTER{2};
    const int RADIUS{3};

    // Stroke caps
    const int ROUND{5};
    const int SQUARE{6};
    const int PROJECT{7};

    // Stroke joins
    const int MITER{8};
    const int BEVEL{9};
    // Already defined const int ROUND;

    const int GRAY{1};
    const int GRAYALPHA{2};
    const int RGB{3};
    const int RGBA{4};
    const int ARGB{5};
    const int HSB{6};
    const int HSBA{7};
    const int ALPHA{8};

    enum Unit
    {
        PX,
        CM,
        MM,
        IN,
        CURRENT
    };

    // Blend mode
    const int REPLACE{0};
    const int BLEND{1 << 0};
    const int ADD{1 << 1};
    const int SUBTRACT{1 << 2};
    const int LIGHTEST{1 << 3};
    const int DARKEST{1 << 4};
    const int DIFFERENCE{1 << 5};
    const int EXCLUSION{1 << 6};
    const int MULTIPLY{1 << 7};
    const int SCREEN{1 << 8};
    const int OVERLAY{1 << 9};
    const int HARD_LIGHT{1 << 10};
    const int SOFT_LIGHT{1 << 11};
    const int DODGE{1 << 12};
    const int BURN{1 << 13};
} // namespace Pyro

#endif
