#ifndef PYRO_UTILS_H_
#define PYRO_UTILS_H_

#include "constants.h"

namespace Pyro
{
    template <typename T>
    T unit2pixels(T value, Unit unit, unsigned int dpi = 72)
    {
        switch (unit)
        {
        case Unit::IN:
            return value * dpi;
        case Unit::CM:
            return (value * float(dpi)) / 2.54;
        case Unit::MM:
            return (value * float(dpi)) / 25.4;
        case Unit::PX:
        default:
            return value;
        }
    };

    template <typename T>
    T pixels2unit(T value, Unit unit, unsigned int dpi = 72)
    {
        switch (unit)
        {
        case Unit::IN:
            return value / dpi;
        case Unit::CM:
            return (value * 2.54) / dpi;
        case Unit::MM:
            return (value * 25.4) / dpi;
        case Unit::PX:
        default:
            return value;
        }
    };

}

#endif
