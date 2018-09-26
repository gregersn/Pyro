#include <pyroimage.h>
#include <pyroconstants.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <FreeImage.h>

namespace Pyro {
    void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
        printf("\n*** ");
        if(fif != FIF_UNKNOWN) {
            printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
        }
        printf("%s", message);
        printf(" ***\n");
    }
    Image::Image() {
        //printf("Creating image\n");
        this->data = nullptr;
        this->cache = nullptr;
    }

    Image::Image(unsigned int width, unsigned int height, unsigned int bpp) {
        //printf("Creating image\n");
        this->pixels_locked = false;
        this->data = nullptr;
        this->cache = nullptr;
        this->_width = width;
        this->_height = height;
        this->bpp = 4;
        this->data = (unsigned char *)malloc(width * height * sizeof(unsigned char) * this->bpp);

    }

    Image::~Image() {
        //printf("Deleting image\n");
        if(this->data != nullptr) {
            free(this->data);
        }
    }
    void Image::save(const std::string &file) {
        FreeImage_Initialise();
        FreeImage_SetOutputMessage(FreeImageErrorHandler);
        FIBITMAP *img = FreeImage_ConvertFromRawBits((unsigned char *)this->data,
                                                     this->_width, this->_height,
                                                     this->_width * this->bpp, this->bpp * 8,
                                                     0xff0000, 0xff00, 0xff, 1);
        
        FreeImage_Save(FIF_PNG, img, file.c_str());
        FreeImage_Unload(img);
        FreeImage_DeInitialise();
    }

    Image* Image::create(unsigned int width, unsigned int height) {
        unsigned int bpp = 4;
        Image *img = new Image(width, height, bpp);
        return img;
    }

    uint32_t* Image::load_pixels() {
        this->pixels_locked = true;
        return (uint32_t *)this->data;
    }

    void *Image::get_pre_multiplied_data() {
        if(this->cache == nullptr) {
            this->cache = malloc(this->_width * this->_height * sizeof(unsigned char) * this->bpp);            
            unsigned char *cache = (unsigned char *)this->cache;
            unsigned char *source = (unsigned char *)this->data;
            
            for(unsigned int i = 0; i < this->_width * this->_height * 4; i += 4) {
                cache[i] = source[i] * source[i + 3] / 255;
                cache[i + 1] = source[i + 1] * source[i + 3] / 255;
                cache[i + 2] = source[i + 2] * source[i + 3] / 255;
                cache[i + 3] = source[i + 3];        
            }
        }
        return this->cache;
    }

    void Image::update_pixels() {
        this->pixels_locked = false;
        /*if(this->cache != nullptr) {
            free(this->cache);
            this->cache = nullptr;
        }*/
    }

    Image* createimage(unsigned int width, unsigned int height, int mode) {
        return new Image(width, height, mode);
    }
}