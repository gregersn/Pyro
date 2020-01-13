#include "pyro/pyroimage.h"
#include "pyro/pyrocolor.h"
#include "pyro/pyroconstants.h"

#include <png.h>

namespace Pyro {
    // Public functions

    Image::Image() {
        this->data = nullptr;
        this->cache = nullptr;
    }

    Image::Image(unsigned int width, unsigned int height, unsigned int channels) {
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
        return loadPNG(filename);
    }

    Image *Image::loadPNG(const std::string &filename) {
        FILE *fp = fopen(filename.c_str(), "rb");
        if(!fp) {
            return nullptr;
        }
        unsigned int number = 8;
        png_const_bytep header = (png_const_bytep)malloc(sizeof(char) * number);
        fread((void *)header, 1, number, fp);
        bool is_png = !png_sig_cmp(header, 0, number);
        if(!is_png) {
            return nullptr;
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(!png_ptr) {
            return nullptr;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
            return nullptr;
        }

        png_infop end_info = png_create_info_struct(png_ptr);
        if(!end_info) {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            return nullptr;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, number);
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
        png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

        unsigned int width = png_get_image_width(png_ptr, info_ptr);
        unsigned int height = png_get_image_height(png_ptr, info_ptr);
        unsigned int channels = png_get_channels(png_ptr, info_ptr);

        Image *img = new Image(width, height, channels);
        unsigned char *d = (unsigned char *)img->get_data();
        for(unsigned int y = 0; y < height; y++) {
            png_bytep row = row_pointers[y];
            unsigned int line = y * width * channels;

            for(unsigned int x = 0; x < width; x++) {
                for(unsigned int ch = 0; ch < channels; ch++) {
                    d[line + x * channels + ch] = row[x * channels + ch];
                }
            }
        }
        return img;
    }

    void Image::save(const std::string &filename) {
        this->save(filename, 72);
    }

    void Image::save(const std::string &filename, unsigned int dpi) {
        return this->savePNG(filename, dpi);
    }

    void Image::savePNG(const std::string &filename, unsigned int dpi) {
        FILE *fp = fopen(filename.c_str(), "wb");
        if(!fp) {
            throw;
        }

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(!png_ptr) {
            throw;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            throw;
        }

        if(setjmp(png_jmpbuf(png_ptr))) throw;

        png_init_io(png_ptr, fp);

        int color_type = 0;
        switch(this->channels) {
            case GRAY:
                color_type = PNG_COLOR_TYPE_GRAY;
                break;
            case GRAYALPHA:
                color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
                break;
            case RGB:
                color_type = PNG_COLOR_TYPE_RGB;
                break;
            case ARGB:
            default:
                color_type = PNG_COLOR_TYPE_RGB_ALPHA;
                break;
        }

        png_set_IHDR(
            png_ptr,
            info_ptr,
            this->width(),
            this->height(),
            8,
            color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
        );

        //png_write_info(png_ptr, info_ptr);

        png_bytep row_pointers[this->height()];
        png_bytep data = (png_bytep)this->get_data();
        for(unsigned int y = 0; y < this->height(); y++) {
            row_pointers[y] = &(data[y * this->width() * this->channels]);
        }

        png_set_rows(png_ptr, info_ptr, row_pointers);
        //png_write_image(png_ptr, row_pointers);
        //png_write_end(png_ptr, NULL);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);

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
        if(this->channels == 4) {
            return (uint32_t *)this->data;
        }
        return nullptr;
    }

    void Image::update_pixels() {
        if(this->channels == 4) {
        }
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
        if(this->channels == 4) {
            unsigned int *pixels = (unsigned int *)this->data;
            return pixels[y * this->_width + x];
        }
        if(this->channels == 3) {
            unsigned char *pixels = (unsigned char *)this->data;
            unsigned int line = y * this->_width * this->channels;
            return 0xff000000 | (pixels[line + x * this->channels + 2] << 16) | (pixels[line + x * this->channels + 1] << 8) | (pixels[line + x * this->channels + 0]);
        }
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

    void Image::set(unsigned int x, unsigned int y, unsigned int c) {
        if(x < this->width() && y < this->height()) {
            unsigned int *pixels = (unsigned int *)this->data;
            pixels[y * this->width() + x] = c;
        }
        
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

        unsigned char *out_pixels = (unsigned char *)out->get_data();
        unsigned char *in_pixels = (unsigned char *)this->get_data();
        for(unsigned int oy = 0; oy < out->height(); oy++) {
            
            unsigned int out_line = oy * out->width() * out->channels;
            unsigned int in_line = (oy / sy) * this->width() * this->channels;

            for(unsigned int ox = 0; ox < out->width(); ox++) {
                unsigned int in_col = (ox / sx) * out->channels;
                for(unsigned int ch = 0; ch < this->channels; ch++)
                    out_pixels[out_line + ox * this->channels + ch] = in_pixels[in_line +  in_col + ch];
            }
        }

        return out;
    }

    Image *Image::resize_bilinear(unsigned int width, unsigned int height) {
        float sx = (float)width / (float)this->width();
        float sy = (float)height / (float)this->height();
        
        Image *out = createimage(width, height, this->channels);

        unsigned char *out_pixels = (unsigned char *)out->get_data();
        unsigned char *in_pixels = (unsigned char *)this->get_data();

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

                for(unsigned int ch = 0; ch < this->channels; ch++) {
                    unsigned char q11 = in_pixels[y1 * this->width() * this->channels + x1 * this->channels + ch];
                    unsigned char q12 = in_pixels[y2 * this->width() * this->channels + x1 * this->channels + ch];
                    unsigned char q21 = in_pixels[y1 * this->width() * this->channels + x2 * this->channels + ch];
                    unsigned char q22 = in_pixels[y2 * this->width() * this->channels + x2 * this->channels + ch];

                    unsigned char xy1 = lerp(q11, q21, x_lerp);
                    unsigned char xy2 = lerp(q12, q22, x_lerp);
                    unsigned char out_color = lerp(xy1, xy2, y_lerp);
                    out_pixels[y * width * this->channels + x * this->channels + ch] = out_color;
                }
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