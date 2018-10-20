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

                Image *img = new Image(width, height, bpp);

                BYTE *d = (BYTE *)img->get_data();
                BYTE *bits = (BYTE *)FreeImage_GetBits(t);
                unsigned int pitch = FreeImage_GetPitch(t);
                for(unsigned int y = 0; y < height; y++) {
                    BYTE *pixel = (BYTE *)bits;
                    for(unsigned int x = 0; x < width; x++) {
                        d[y * width + x] = pixel[FI_RGBA_RED];
                        d[y * width + x + 1] = pixel[FI_RGBA_GREEN];
                        d[y * width + x + 2] = pixel[FI_RGBA_BLUE];
                        d[y * width + x + 3] = pixel[FI_RGBA_ALPHA];
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