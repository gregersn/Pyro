#include "pyro/pyro.h"
#include <cstdlib>

namespace Pyro
{
    Graphics *pg{nullptr};
    unsigned int width{640};
    unsigned int height{480};
    uint32_t *pixels{nullptr};

    void exit()
    {
        if (pg != nullptr)
        {
            delete pg;
        }
    }

    float size_multiplier(Unit unit, unsigned int dpi = 72)
    {
        switch (unit)
        {
        case Unit::MM:
            return dpi / 25.4;
            break;
        case Unit::CM:
            return dpi / 2.54;
            break;
        case Unit::IN:
            return dpi;
            break;
        case Unit::PX:
        default:
            return 1.0f;
            break;
        }
    };

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi)
    {
        float multiplier{size_multiplier(unit, dpi)};

        if (pg == nullptr)
        {
            std::atexit(exit);
        }

        if (pg != nullptr)
        {
            delete pg;
        }
        Pyro::width = (unsigned int)width * multiplier;
        Pyro::height = (unsigned int)height * multiplier;
        pg = creategraphics(width, height, GraphicsMode::CAIRO);
        // , 100, Pyro::Unit::MM
    }

    void loadpixels()
    {
        pixels = pg->load_pixels();
    }

    void updatepixels()
    {
        pixels = nullptr;
    }

}
