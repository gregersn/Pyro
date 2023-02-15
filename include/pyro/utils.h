#pragma once

#include "constants.h"

namespace Pyro
{
    template <typename T>
    T unit2pixels(T value, Unit unit, unsigned int dpi = 72)
    {
        switch (unit)
        {
        case Unit::in:
            return value * dpi;
        case Unit::cm:
            return (value * float(dpi)) / 2.54;
        case Unit::mm:
            return (value * float(dpi)) / 25.4;
        case Unit::px:
        default:
            return value;
        }
    };

    template <typename T>
    T pixels2unit(T value, Unit unit, unsigned int dpi = 72)
    {
        switch (unit)
        {
        case Unit::in:
            return value / dpi;
        case Unit::cm:
            return (value * 2.54) / dpi;
        case Unit::mm:
            return (value * 25.4) / dpi;
        case Unit::px:
        default:
            return value;
        }
    };
}
