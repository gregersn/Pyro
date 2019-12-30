#include <pyro/pyroimage.h>
#include <pyro/pyrocolor.h>
#include <pyro/pyroconstants.h>

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

    Image::Image(unsigned int width, unsigned int height, unsigned int channels) {
        //printf("Creating image\n");
        this->pixels_locked = false;
        this->data = nullptr;
        this->cache = nullptr;
        this->_width = width;
        this->_height = height;
        this->channels = channels;
        this->data = (unsigned char *)malloc(width * height * sizeof(unsigned char) * this->channels);

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
        unsigned int channels = 4;
        Image *img = new Image(width, height, channels);
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
                                                     this->_width * this->channels, this->channels * 8,
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
            this->cache = malloc(this->_width * this->_height * sizeof(unsigned char) * this->channels);            
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
        throw;
    }

    uint32_t Image::get(unsigned int x, unsigned int y) {
        unsigned int *pixels = (unsigned int *)this->data;
        return pixels[y * this->_width + x];
    }

    Image* Image::get(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        Image *out = createimage(width, height, this->channels);
        for(unsigned int ty = 0; ty < height; ty++){
            for(unsigned int tx = 0; tx < width; tx++) {
                unsigned int pos = (y + ty) * this->_width + (x + tx);
                unsigned int pixel = ((unsigned int *)this->data)[pos];
                ((unsigned int *)out->data)[ty * width + tx] = pixel;
            }
        }
        return out;
    }

    Image *Image::resize(unsigned int width, unsigned int height, RESIZEMETHOD method) {
        if(width == 0) {
            width = uint((float)this->width() / ((float)this->height() / (float)height));
        } else if(height == 0) {
            height = uint((float)this->height() / ((float)this->width() / (float)width));
        }
        switch(method) {
            case BILINEAR:
                return this->resize_bilinear(width, height);
            case NEAREST:
            default:
                return this->resize_nearest(width, height);
            
        }
    }
    Image *Image::resize_nearest(unsigned int width, unsigned int height) {
        float sx = width / this->width();
        float sy = height / this->height();
        
        Image *out = createimage(width, height, this->channels);

        unsigned int *out_pixels = out->load_pixels();
        unsigned int *in_pixels = this->load_pixels();
        for(unsigned int oy = 0; oy < out->height(); oy++) {
            unsigned int out_line = oy * out->width();
            unsigned int in_line = oy / sy * this->width();
            for(unsigned int ox = 0; ox < out->width(); ox++) {
                unsigned int in_col = ox / sx;
                out_pixels[out_line + ox] = in_pixels[in_line +  in_col];
            }
        }
        out->update_pixels();
        this->update_pixels();
        
        return out;
    }

    Image *Image::resize_bilinear(unsigned int width, unsigned int height) {
        float sx = (float)width / (float)this->width();
        float sy = (float)height / (float)this->height();
        
        Image *out = createimage(width, height, this->channels);
        unsigned int *out_pixels = out->load_pixels();
        unsigned int *in_pixels = this->load_pixels();

        for(uint y = 0; y < height; y++) {
            float in_y = (float)y / (float)sy;
            uint y1 = (uint)in_y;
            uint y2 = (uint)in_y + 1;
            float y_lerp = in_y  - y1;
            for(uint x = 0; x < width; x++) {
                float in_x = (float)x / (float)sx;
                uint x1 = (uint)in_x;
                uint x2 = (uint)in_x + 1;
                float x_lerp = in_x - x1;

                Color q11 = Color::from_uint(in_pixels[y1 * this->width() + x1]);
                Color q12 = Color::from_uint(in_pixels[y2 * this->width() + x1]);
                Color q21 = Color::from_uint(in_pixels[y1 * this->width() + x2]);
                Color q22 = Color::from_uint(in_pixels[y2 * this->width() + x2]);

                Color xy1 = q11.lerp(q21, x_lerp);
                Color xy2 = q12.lerp(q22, x_lerp);
                Color out_color = xy1.lerp(xy2, y_lerp);
                out_pixels[y * width + x] = out_color.to_uint();
            }
        }

        out->update_pixels();
        this->update_pixels();
        return out;
    }


    // Utility functions

    Image* createimage(unsigned int width, unsigned int height, int channels) {
        return new Image(width, height, channels);
    }
}