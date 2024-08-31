#include "pyro/pyro.h"
#include <cstdlib>

namespace Pyro
{
    Graphics *pg{nullptr};
    unsigned int width{640};
    unsigned int height{480};
    uint32_t *pixels{nullptr};

    float real_width() { return pg->real_width(); }
    float real_height() { return pg->real_height(); }

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

    void size(unsigned int width, unsigned int height)
    {
        size(width, height, Unit::PX, 72);
    }

    void size(float width, float height, Unit unit, unsigned int dpi)
    {
        if (pg == nullptr)
        {
            std::atexit(exit);
        }

        if (pg != nullptr)
        {
            delete pg;
        }
        pg = creategraphics(width, height, GraphicsMode::CAIRO);
        pg->set_dpi(dpi);
        pg->set_unit(unit);
        pg->init();
        Pyro::width = pg->width();
        Pyro::height = pg->height();
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
