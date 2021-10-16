%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/constants.h"
%End
    const double PI;
    const double TWO_PI;
    const double HALF_PI;
    const double QUARTER_PI;

    const int OPEN;
    const int CLOSE;

    // Arc drawing styles
    const int PIE;
    const int CHORD;
    
    // Shape drawing modes (rect, ellipse, image, shape)
    const int CORNER;
    const int CORNERS;
    const int CENTER;
    const int RADIUS;
    
    // Stroke caps
    const int ROUND;
    const int SQUARE;
    const int PROJECT;

    // Stroke joins
    const int MITER;
    const int BEVEL;


    const int HSB;
    const int HSBA;
    const int RGB;
    const int RGBA;
};