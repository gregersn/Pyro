#include <pyroimage.h>
#include <pyroconstants.h>

#include <FreeImage.h>

namespace Pyro {
    static bool FI_initialised = false;

    void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
        printf("\n*** ");
        if(fif != FIF_UNKNOWN) {
            printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
        }
        printf("%s", message);
        printf(" ***\n");
    }

    void FI_init() {
        if(!FI_initialised) {
            printf("Initialize FreeImage\n");
            FreeImage_Initialise();
            FreeImage_SetOutputMessage(FreeImageErrorHandler);
            std::atexit(FreeImage_DeInitialise);
            FI_initialised = true;
        }
    }

    // Public functions

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
        if(this->cache != nullptr) {
            free(this->cache);
        }
        if(this->data != nullptr) {
            free(this->data);
        }
    }

    Image* Image::create(unsigned int width, unsigned int height) {
        unsigned int bpp = 4;
        Image *img = new Image(width, height, bpp);
        return img;
    }

    Image* Image::load(const std::string &filename) {
        if(!FI_initialised) {
            FI_init();
        }

        FREE_IMAGE_FORMAT fif  = FIF_UNKNOWN;
        fif = FreeImage_GetFileType(filename.c_str(), 0);
        if(fif == FIF_UNKNOWN) {
            fif = FreeImage_GetFIFFromFilename(filename.c_str());
        }

        if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
            FIBITMAP *bitmap = FreeImage_Load(fif, filename.c_str(), 0);
            if(bitmap) {
                unsigned int width = FreeImage_GetWidth(bitmap);
                unsigned int height = FreeImage_GetHeight(bitmap);
                unsigned int bpp = FreeImage_GetBPP(bitmap);

                FIBITMAP *t = FreeImage_ConvertTo32Bits(bitmap);
                FreeImage_Unload(bitmap);

                bpp = FreeImage_GetBPP(t);

                Image *img = new Image(width, height, bpp / 4);

                BYTE *d = (BYTE *)img->get_data();
                BYTE *bits = (BYTE *)FreeImage_GetBits(t);
                unsigned int pitch = FreeImage_GetPitch(t);
                for(unsigned int y = 0; y < height; y++) {
                    BYTE *pixel = (BYTE *)bits;
                    for(unsigned int x = 0; x < width; x++) {
                        d[(height - y - 1) * width * 4 + x * 4 + 3] = pixel[FI_RGBA_ALPHA];
                        d[(height - y - 1) * width * 4 + x * 4 + 2] = pixel[FI_RGBA_RED];
                        d[(height - y - 1) * width * 4 + x * 4 + 1] = pixel[FI_RGBA_GREEN];
                        d[(height - y - 1) * width * 4 + x * 4 + 0] = pixel[FI_RGBA_BLUE];
                        pixel += 4;
                    }
                    bits += pitch;
                }

                FreeImage_Unload(t); 
                return img;
            }
        }
        return nullptr;
    }


    void Image::save(const std::string &filename) {
        this->save(filename, 72);
    }

    void Image::save(const std::string &filename, unsigned int dpi) {
        if(!FI_initialised) {
            FI_init();
        }

        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
        if(fif == FIF_UNKNOWN) {
            printf("Unknown filetype, won't save!\n");
            return;
        }

        FIBITMAP *img = FreeImage_ConvertFromRawBits((unsigned char *)this->data,
                                                     this->_width, this->_height,
                                                     this->_width * this->bpp, this->bpp * 8,
                                                     0xff0000, 0xff00, 0xff, 1);

        unsigned int dots_per_meter = (unsigned int)((dpi * 100) / 2.54);
        FreeImage_SetDotsPerMeterX(img, dots_per_meter);
        FreeImage_SetDotsPerMeterY(img, dots_per_meter);
        
        FreeImage_Save(fif, img, filename.c_str());
        FreeImage_Unload(img);
    }

    // Pixel access functions
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

    uint32_t* Image::load_pixels() {
        this->pixels_locked = true;
        return (uint32_t *)this->data;
    }

    void Image::update_pixels() {
        this->pixels_locked = false;
        /*if(this->cache != nullptr) {
            free(this->cache);
            this->cache = nullptr;
        }*/
    }

    unsigned int Image::operator[] (unsigned int index) {
        if(index < this->_width * this->_height) {
            return ((unsigned int *)this->data)[index];
        }
    }

    uint32_t Image::get(unsigned int x, unsigned int y) {
        unsigned int *pixels = (unsigned int *)this->data;
        return pixels[y * this->_width + x];
    }

    Image* Image::get(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        Image *out = createimage(width, height, this->bpp);
        for(unsigned int ty = 0; ty < height; ty++){
            for(unsigned int tx = 0; tx < width; tx++) {
                unsigned int pos = (y + ty) * this->_width + (x + tx);
                unsigned int pixel = ((unsigned int *)this->data)[pos];
                ((unsigned int *)out->data)[ty * width + tx] = pixel;
            }
        }
        return out;
    }

    // Utility functions

    Image* createimage(unsigned int width, unsigned int height, int bpp) {
        return new Image(width, height, bpp);
    }
}