#include <pyro.h>
#include <cstdlib>

namespace Pyro {
    Graphics *pg = NULL;
    unsigned int width;
    unsigned int height;
    uint32_t *pixels = NULL;
    
    void exit() {
        if(pg != NULL) {
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
        if(pg == NULL) {
            std::atexit(exit);
        }

        if(pg != NULL) {
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
        pixels = NULL;
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b) {
        return 0xff000000 | r << 16 | g << 8 | b;
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
        return a << 24 | r << 16 | g << 8 | b;
    }

}
