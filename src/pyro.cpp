#include <pyro.h>
#include <cstdlib>

namespace Pyro {
    Graphics *pg = nullptr;
    unsigned int width = 640;
    unsigned int height = 480;
    uint32_t *pixels = nullptr;
    
    void exit() {
        if(pg != nullptr) {
            delete pg;
        }
    }

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi) {
        float multiplier = 1.0f;
        switch(unit) {
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

        return size((unsigned int)width * multiplier, (unsigned int)height * multiplier);
    }


    void size(unsigned int w, unsigned int h) {
        if(pg == nullptr) {
            std::atexit(exit);
        }

        if(pg != nullptr) {
            delete pg;
        }
        width = w;
        height = h;
        pg = Graphics::create(width, height);
    }

    void loadpixels() {
        pixels = pg->load_pixels();
    }

    void updatepixels() {
        pixels = nullptr;
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b) {
        return 0xff000000 | r << 16 | g << 8 | b;
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
        return a << 24 | r << 16 | g << 8 | b;
    }

}
