#ifndef PYROIMAGE_H
#define PYROIMAGE_H

#include <string>
#include <stdint.h>

namespace Pyro {
    class Image {
        private:
            bool pixels_locked;
            void *cache;

        protected:
            void *data;
            unsigned int bpp;
            unsigned int _width;
            unsigned int _height;

        public:
            unsigned int width() { return this->_width; };
            unsigned int height() { return this->_height; };

            Image();
            Image(unsigned int width, unsigned int height, unsigned int bpp);
            ~Image();
            void save(const std::string &file);
            void *get_data() { return this->data; };
            void *get_pre_multiplied_data();
            uint32_t *load_pixels();
            void update_pixels();
            //static Image load(const std::wstring &file);
            static Image *create(unsigned int width, unsigned int height);
    };

    Image *createimage(unsigned int width, unsigned int height, int mode);
}
#endif