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
