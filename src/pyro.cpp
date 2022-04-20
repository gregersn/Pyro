#include "pyro/pyro.h"
#include <cstdlib>

namespace Pyro
{
    Graphics *pg = nullptr;
    unsigned int width = 640;
    unsigned int height = 480;
    uint32_t *pixels = nullptr;

    void exit()
    {
        if (pg != nullptr)
        {
            delete pg;
        }
    }

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi)
    {
        float multiplier = 1.0f;
        switch (unit)
        {
        case Unit::mm:
            multiplier = dpi / 25.4;
            break;
        case Unit::cm:
            multiplier = dpi / 2.54;
            break;
        case Unit::in:
            multiplier = dpi;
            break;
        case Unit::px:
        default:
            break;
        }

        unsigned int w = (unsigned int)width * multiplier;
        unsigned int h = (unsigned int)height * multiplier;

        if (pg == nullptr)
        {
            std::atexit(exit);
        }

        if (pg != nullptr)
        {
            delete pg;
        }
        Pyro::width = w;
        Pyro::height = h;
        pg = Graphics::create(width, height, GraphicsMode::CAIRO, dpi = dpi, unit = unit);
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
