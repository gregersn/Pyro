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
            bool pixels_locked;
            void *cache;

            Image *resize_nearest(unsigned int width, unsigned int height);
            Image *resize_bilinear(unsigned int width, unsigned int height);

        protected:
            void *data;
            unsigned int _width;
            unsigned int _height;

        public:
            unsigned int channels;
            unsigned int width() { return this->_width; };
            unsigned int height() { return this->_height; };

            Image();
            Image(unsigned int width, unsigned int height, unsigned int channels);
            ~Image();

            static Image *create(unsigned int width, unsigned int height);

            static Image *load(const std::string &filename);
            void save(const std::string &filename);
            void save(const std::string &filename, unsigned int dpi);


            // Pixel access
            void *get_data() { return this->data; };
            void *get_pre_multiplied_data();
            uint32_t *load_pixels();
            void update_pixels();
            unsigned int operator[] (unsigned int);
            uint32_t get(unsigned int x, unsigned int y);
            Image *get(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

            // Image manipulation functions
            Image *resize(unsigned int width, unsigned int height) { return this->resize(width, height, NEAREST); };
            Image *resize(unsigned int width, unsigned int height, RESIZEMETHOD method);
    };

    Image *createimage(unsigned int width, unsigned int height, int channels);
}
#endif