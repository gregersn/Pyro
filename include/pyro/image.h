#ifndef PYROIMAGE_H
#define PYROIMAGE_H

#include <string>
#include <cstdint>
#include "constants.h"

namespace Pyro
{
    enum FILETYPE
    {
        JPEG,
        PNG,
        TIFF
    };
    enum RESIZEMETHOD
    {
        NEAREST,
        BILINEAR
    };

    enum BLENDMODE
    {
        BLEND,
        ADD,
        SUBTRACT,
        LIGHTEST,
        DARKEST,
        DIFFERENCE,
        EXCLUSION,
        MULTIPLY,
        SCREEN,
        OVERLAY,
        HARD_LIGHT,
        SOFT_LIGHT,
        DODGE,
        BURN
    };
    class Image
    {
    private:
        bool pixels_locked = false;
        void *cache = nullptr;
        uint32_t *data = nullptr;
        unsigned int dpi = 72;

        Image *resize_nearest(unsigned int width, unsigned int height);
        Image *resize_bilinear(unsigned int width, unsigned int height);

        bool intersect(int sx1, int sy1, int sx2, int sy2, int dx1, int dy1, int dx2, int dy2);
        void blit_resize(Image *img, int srcX1, int srcY1, int srcX2, int srcY2,
                         uint32_t *destpixels, unsigned int screenW, unsigned int screenH,
                         int destX1, int destY1, int destX2, int destY2,
                         unsigned int mode);

    protected:
        unsigned int _pixelwidth = 0;
        unsigned int _pixelheight = 0;
        unsigned int density = 1;

        bool modified = false;
        int mx1 = 0, my1 = 0, mx2 = 0, my2 = 0;

    public:
        unsigned int format = ARGB;
        unsigned int width() { return this->_pixelwidth; };
        unsigned int height() { return this->_pixelheight; };
        unsigned int channels();
        Unit unit = Unit::px;

        Image(const Image &in);
        Image();
        Image(unsigned int width, unsigned int height, unsigned int format = RGB, unsigned int factor = 1, unsigned int dpi = 72, Unit unit = Unit::px);
        void init(unsigned int width, unsigned int height, unsigned int format = RGB, unsigned int factor = 1, unsigned int dpi = 72, Unit unit = Unit::px);

        Image &operator=(const Image &in);
        virtual ~Image();

        static Image *create(unsigned int width, unsigned int height);
        static Image *load(const std::string &filename);
        static Image *loadPNG(const std::string &filename);
        static Image *loadJPEG(const std::string &filename);

        void save(const std::string &filename);
        void save(const std::string &filename, unsigned int dpi);

        void savePNG(const std::string &filename) { this->savePNG(filename, this->dpi); };
        void savePNG(const std::string &filename, unsigned int dpi);

        unsigned int get_dpi() { return this->dpi; }

        // Pixel access
        void *get_data() const { return this->data; };
        void *get_pre_multiplied_data();
        uint32_t *load_pixels();
        uint8_t *load_bytes();
        void update_pixels();
        void update_pixels(int x, int y, int w, int h);
        unsigned int operator[](unsigned int);

        Image *get();
        uint32_t get(unsigned int x, unsigned int y);
        Image *get(int x, int y, int width, int height);

        void set(unsigned int x, unsigned int y, unsigned int c);
        void set(int x, int y, Image *img);
        void set(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int c);

        void blend(Image *src, int sx, int sy, unsigned int sw, unsigned int sh, int dx, int dy, unsigned int dw, unsigned int dh, BLENDMODE mode);

        void mask(Image *mask);

        // Image manipulation functions
        Image *resize(unsigned int width, unsigned int height, RESIZEMETHOD method = RESIZEMETHOD::NEAREST);

        Image *convert(unsigned int format);

        Image *rotate(double angle, RESIZEMETHOD method = RESIZEMETHOD::BILINEAR);
        Image *rotate_nearest(double angle);
        Image *rotate_bilinear(double angle);
    };

    Image *createimage(unsigned int width, unsigned int height, int format = RGB, unsigned int dpi = 72, Unit unit = Unit::px);
}
#endif
