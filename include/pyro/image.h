#ifndef PYROIMAGE_H
#define PYROIMAGE_H

#include <string>
#include <filesystem>
#include <cstdint>
#include "constants.h"
#include "utils.h"

namespace Pyro
{
    enum FileType
    {
        JPEG,
        PNG,
        TIFF
    };
    enum ResizeMethod
    {
        NEAREST,
        BILINEAR
    };

    class Image
    {
      private:
        bool pixels_locked{false};
        void *cache{nullptr};
        uint32_t *data{nullptr};
        unsigned int dpi{72};

        Image *resize_nearest(unsigned int width, unsigned int height);
        Image *resize_bilinear(unsigned int width, unsigned int height);

        bool intersect(int sx1, int sy1, int sx2, int sy2, int dx1, int dy1, int dx2, int dy2);
        void blit_resize(Image *img, int srcX1, int srcY1, int srcX2, int srcY2,
                         uint32_t *destpixels, int screenW, int screenH,
                         int destX1, int destY1, int destX2, int destY2,
                         unsigned int mode);

      protected:
        unsigned int _width{0};
        unsigned int _height{0};
        unsigned int _pixelwidth{0};
        unsigned int _pixelheight{0};
        unsigned int density{1};
        float pixel_multiplier{1.0f};

        bool modified{true};
        int mx1{0}, my1{0}, mx2{0}, my2{0};
        bool initialized{false};

      public:
        unsigned int format{ARGB};
        unsigned int width() { return this->_pixelwidth; };
        unsigned int height() { return this->_pixelheight; };
        unsigned int channels();
        float real_width() { return pixels2unit(this->_pixelwidth, this->unit, this->dpi); };
        float real_height() { return pixels2unit(this->_pixelwidth, this->unit, this->dpi); };
        Unit unit{Unit::PX};

        Image(const Image &in);
        Image();
        Image(unsigned int width, unsigned int height, unsigned int format = RGB, unsigned int factor = 1, unsigned int dpi = 72, Unit unit = Unit::PX);
        virtual void init();

        Image &operator=(const Image &in);
        virtual ~Image();

        static Image *load(const std::filesystem::path &filename);
        static Image *loadPNG(const std::filesystem::path &filename);
        static Image *loadJPEG(const std::filesystem::path &filename);

        void save(const std::filesystem::path &filename);
        void save(const std::filesystem::path &filename, unsigned int dpi);

        void savePNG(const std::filesystem::path &filename) { this->savePNG(filename, this->dpi); };
        void savePNG(const std::filesystem::path &filename, unsigned int dpi);

        unsigned int get_dpi() { return this->dpi; }
        void set_dpi(int dpi);

        void set_unit(Unit unit);

        // Pixel access
        void *get_data() const { return this->data; };
        void *get_pre_multiplied_data();
        uint32_t *load_pixels();
        uint8_t *load_bytes();
        void update_pixels();
        void update_pixels(int x, int y, int w, int h);
        unsigned int operator[](unsigned int);

        Image *get();
        uint32_t get(int x, int y);
        Image *get(int x, int y, int width, int height);

        void set(unsigned int x, unsigned int y, unsigned int c);
        void set(int x, int y, Image *img);
        void set(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int c);

        void blend(Image *src, int sx, int sy, unsigned int sw, unsigned int sh, int dx, int dy, unsigned int dw, unsigned int dh, int mode);

        void mask(Image *mask);

        // Image manipulation functions
        Image *resize(unsigned int width, unsigned int height, ResizeMethod method = ResizeMethod::NEAREST);

        Image *convert(unsigned int format);

        Image *rotate(double angle, ResizeMethod method = ResizeMethod::BILINEAR);
        Image *rotate_nearest(double angle);
        Image *rotate_bilinear(double angle);
    };

    Image *createimage(unsigned int width, unsigned int height, int format = RGB);
} // namespace Pyro
#endif
