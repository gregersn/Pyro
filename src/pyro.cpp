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

    void curvedetail(int segments) { pg->curvedetail(segments); }

    Color fill() { return pg->fill(); };
    Color fill(Color const &c) { return pg->fill(c); };
    Color fill(float c, float a) { return pg->fill(c, c, c, a); };
    Color fill(float r, float g, float b, float a) { return pg->fill(r, g, b, a); };
    Color fill(int c, int a) { return pg->fill(c, a); };
    Color fill(int r, int g, int b, int a) { return pg->fill(r, g, b, a); };

    void nofill() { pg->nofill(); };
    void nostroke() { pg->nostroke(); };

    Color stroke() { return pg->stroke(); }
    Color stroke(Color const &c) { return pg->stroke(c); };
    Color stroke(float c, float a) { return pg->stroke(c, c, c, a); };
    Color stroke(float r, float g, float b, float a) { return pg->stroke(r, g, b, a); };
    Color stroke(int c, int a) { return pg->stroke(c, a); };
    Color stroke(int r, int g, int b, int a) { return pg->stroke(r, g, b, a); };

}
