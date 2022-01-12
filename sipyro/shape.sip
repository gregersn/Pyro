%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/shape.h"
%End
    float bezierpoint(float a, float b, float c, float d, float t);
    void curveresolution(unsigned int res);
    class Shape;
};