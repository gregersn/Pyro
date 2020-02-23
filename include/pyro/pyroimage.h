#ifndef PYROIMAGE_H
#define PYROIMAGE_H

#include <string>

namespace Pyro {
    enum FILETYPE {
        JPEG, PNG, TIFF 
    };
    enum RESIZEMETHOD {
        NEAREST, BILINEAR
    };
    class Image {
        private:
            bool pixels_locked = false;
            void *cache = nullptr;
            unsigned int dpi =  72;

            Image *resize_nearest(unsigned int width, unsigned int height);
            Image *resize_bilinear(unsigned int width, unsigned int height);

        protected:
            void *data = nullptr;
            unsigned int _width = 0;
            unsigned int _height = 0;

        public:
            unsigned int channels = 4;
            unsigned int width() { return this->_width; };
            unsigned int height() { return this->_height; };

            Image();
            Image(const Image &in);
            Image(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
            Image & operator=(const Image &in);
            virtual ~Image();

            static Image *create(unsigned int width, unsigned int height);
            static Image *load(const std::string &filename);
            static Image *loadPNG(const std::string &filename);

            void save(const std::string &filename);
            void save(const std::string &filename, unsigned int dpi);

            void savePNG(const std::string &filename) { this->savePNG(filename, 72); };
            void savePNG(const std::string &filename, unsigned int dpi);


            // Pixel access
            void *get_data() { return this->data; };
            void *get_pre_multiplied_data();
            uint32_t *load_pixels();
            uint8_t *load_bytes();
            void update_pixels();
            unsigned int operator[] (unsigned int);
            Image *get();
            uint32_t get(unsigned int x, unsigned int y);
            Image *get(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
            void set(unsigned int x, unsigned int y, unsigned int c);

            // Image manipulation functions
            Image *resize(unsigned int width, unsigned int height) { return this->resize(width, height, NEAREST); };
            Image *resize(unsigned int width, unsigned int height, RESIZEMETHOD method);

            Image *convert(unsigned int channels);
    };

    Image *createimage(unsigned int width, unsigned int height);
    Image *createimage(unsigned int width, unsigned int height, int channels);
    Image *createimage(unsigned int width, unsigned int height, int channels, unsigned int dpi);
}
#endif
