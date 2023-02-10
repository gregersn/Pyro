#include "pyro/image.h"
#include "pyro/color.h"
#include "pyro/constants.h"
#include "pyro/utils.h"

#include <jpeglib.h>
#include <setjmp.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <png.h>

#define ALPHA(x) ((x & 0xff000000) >> 24)
#define RED(x) ((x & 0xff0000) >> 16)
#define GREEN(x) ((x & 0xff00) >> 8)
#define BLUE(x) ((x & 0xff))

struct ErrorManager
{
    jpeg_error_mgr defaultErrorManager;
    jmp_buf jumpBuffer;
};

void ErrorExit(j_common_ptr cinfo)
{
    // cinfo->err is actually a pointer to my_error_mgr.defaultErrorManager, since pub
    // is the first element of my_error_mgr we can do a sneaky cast
    ErrorManager *pErrorManager{(ErrorManager *)cinfo->err};
    (*cinfo->err->output_message)(cinfo); // print error message (actually disabled below)
    longjmp(pErrorManager->jumpBuffer, 1);
}

void OutputMessage(j_common_ptr cinfo)
{
    // disable error messages
    /*char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);
    fprintf(stderr, "%s\n", buffer);*/
}

std::string get_extension(std::string filename)
{
    // Find file extension
    // https://stackoverflow.com/a/51992/7097
    std::string::size_type idx;
    idx = filename.rfind('.');

    if (idx != std::string::npos)
    {
        std::string extension = filename.substr(idx);
        return extension;
    }
    return std::string("");
}

namespace Pyro
{
    // fixed point precision is limited to 15 bits!!
    static const int64_t PRECISIONB{15};
    static const int64_t PRECISIONF{1 << PRECISIONB};
    static const int64_t PREC_MAXVAL{PRECISIONF - 1};
    static const int64_t PREC_ALPHA_SHIFT{24 - PRECISIONB};
    static const int64_t PREC_RED_SHIFT{16 - PRECISIONB};
    static const uint32_t ALPHA_MASK{0xff000000};
    static const uint32_t RED_MASK{0x00ff0000};
    static const uint32_t GREEN_MASK{0x0000ff00};
    static const uint32_t BLUE_MASK{0x000000ff};

    static const uint32_t RB_MASK{0x00FF00FF};
    static const uint32_t GN_MASK{0x0000FF00};

    uint32_t multiply_alpha(uint32_t c)
    {
        return int(BLUE(c) * ALPHA(c) / 255) |
               (int(GREEN(c) * ALPHA(c) / 255) << 8) |
               (int(RED(c) * ALPHA(c) / 255) << 16) |
               (ALPHA(c) << 24);
    }

    // Blend functions
    uint32_t aoverb(uint32_t a, uint32_t b)
    {
        /*
         *   Straight alpha
         *   (source.RGB * source.A) +  (dest.RGB * (1 - source.A))
         *
         *   Premultiplied alpha:
         *   source.RGB + (dest.RGB * (1 - source.A)))
         *
         */
        a = multiply_alpha(a);
        b = multiply_alpha(b);

        uint32_t out =
            int(ALPHA(a) + ALPHA(b) * (1.0 - ALPHA(a) / 255.0)) << 24 |
            int(RED(a) + RED(b) * (1.0 - ALPHA(a) / 255.0)) << 16 |
            int(GREEN(a) + GREEN(b) * (1.0 - ALPHA(a) / 255.0)) << 8 |
            int(BLUE(a) + BLUE(b) * (1.0 - ALPHA(a) / 255.0));

        return out;
    }

    // Public functions

    Image::Image(const Image &in)
    {
        this->_pixelheight = in._pixelheight;
        this->_pixelwidth = in._pixelwidth;
        this->dpi = in.dpi;
        this->pixels_locked = false;
        this->format = in.format;
        this->data = nullptr;
        this->cache = nullptr;
        this->data = (uint32_t *)::operator new(this->_pixelwidth *this->_pixelheight * sizeof(uint32_t));
        std::memcpy(this->get_data(), in.get_data(), this->_pixelwidth * this->_pixelheight * sizeof(uint32_t));
    }

    Image::Image()
    {
        format = ARGB;
        density = 1;
    }

    Image::Image(unsigned int width, unsigned int height, unsigned int format, unsigned int factor, unsigned int dpi, Unit unit)
    {
        init(width, height, format, factor, dpi, unit);
    }

    void Image::init(unsigned int width, unsigned int height, unsigned int format, unsigned int factor, unsigned int dpi, Unit unit)
    {
        this->dpi = dpi;
        this->density = factor;
        this->unit = unit;
        this->_pixelwidth = unit2pixels(width, unit, dpi) * factor;
        this->_pixelheight = unit2pixels(height, unit, dpi) * factor;
        this->format = format;

        this->data = (uint32_t *)::operator new(this->_pixelwidth *this->_pixelheight * sizeof(uint32_t));
    }

    Image &Image::operator=(const Image &in)
    {
        if (this == &in)
        {
            return *this;
        }
        if (this->get_data() != nullptr)
            ::operator delete(this->get_data());

        this->_pixelwidth = in._pixelwidth;
        this->_pixelheight = in._pixelheight;
        this->dpi = in.dpi;
        this->pixels_locked = false;
        this->format = in.format;
        this->data = nullptr;
        this->cache = nullptr;
        this->data = (uint32_t *)::operator new(this->_pixelwidth *this->_pixelheight * sizeof(uint32_t));
        std::memcpy(this->get_data(), in.get_data(), this->_pixelwidth * this->_pixelheight * sizeof(uint32_t));

        return *this;
    }

    Image::~Image()
    {
        if (this->cache != nullptr)
        {
            ::operator delete(this->cache);
        }
        if (this->data != nullptr)
        {
            ::operator delete(this->data);
        }
    }

    unsigned int Image::channels()
    {
        switch (this->format)
        {
        case HSB:
        case RGB:
            return 3;

        case GRAY:
        case ALPHA:
            return 1;

        case HSBA:
        case RGBA:
        default:
            return 4;
        }
    }

    Image *Image::create(unsigned int width, unsigned int height)
    {
        unsigned int format = ARGB;
        Image *img = new Image(width, height, format, 1);
        return img;
    }

    Image *Image::load(const std::string &filename)
    {
        if (access(filename.c_str(), F_OK) == -1)
        {
            std::cerr << "File not accessible: " << filename << "\n";
            return nullptr;
        }

        std::string extension = get_extension(filename);
        std::transform(extension.begin(), extension.end(), extension.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        if (extension == ".jpeg" || extension == ".jpg")
        {
            return loadJPEG(filename);
        }
        else if (extension == ".png")
        {
            return loadPNG(filename);
        }

        return nullptr;
    }

    Image *Image::loadPNG(const std::string &filename)
    {
        FILE *fp = fopen(filename.c_str(), "rb");
        if (!fp)
        {
            return nullptr;
        }
        unsigned int number = 8;
        png_const_bytep header = (png_const_bytep)malloc(sizeof(char) * number);
        fread((void *)header, 1, number, fp);
        bool is_png = !png_sig_cmp(header, 0, number);
        if (!is_png)
        {
            fclose(fp);
            return nullptr;
        }

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            fclose(fp);
            return nullptr;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
            fclose(fp);
            return nullptr;
        }

        png_infop end_info = png_create_info_struct(png_ptr);
        if (!end_info)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
            fclose(fp);
            return nullptr;
        }

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, number);
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
        png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

        unsigned int width = png_get_image_width(png_ptr, info_ptr);
        unsigned int height = png_get_image_height(png_ptr, info_ptr);
        unsigned int channels = png_get_channels(png_ptr, info_ptr);
        unsigned int dpi = png_get_pixels_per_inch(png_ptr, info_ptr);

        unsigned int format = ARGB;
        if (channels == 4)
        {
            format = ARGB;
        }
        else if (channels == 3)
        {
            format = RGB;
        }
        else if (channels == 2)
        {
            format = GRAYALPHA;
        }
        else if (channels == 1)
        {
            format = ALPHA;
        }
        Image *img = new Image(width, height, format, 1, dpi);
        uint32_t *d = (uint32_t *)img->load_pixels();
        uint32_t col = 0xffbada55;
        for (unsigned int y = 0; y < height; y++)
        {
            png_bytep row = row_pointers[y];
            unsigned int line = y * width;

            for (unsigned int x = 0; x < width; x++)
            {
                if (channels == 4)
                {
                    col = (row[x * channels + 3] << 24) | (row[x * channels + 2] << 16) | (row[x * channels + 1] << 8) | (row[x * channels + 0]);
                }
                else if (channels == 3)
                {
                    col = 0xff000000 | (row[x * channels + 2] << 16) | (row[x * channels + 1] << 8) | (row[x * channels + 0]);
                }
                else if (channels == 2)
                {
                    col = (row[x * channels + 1] << 24) | (row[x * channels + 0] << 16) | (row[x * channels + 0] << 8) | (row[x * channels + 0]);
                }
                else if (channels == 1)
                {
                    col = 0xff000000 | (row[x * channels + 0] << 16) | (row[x * channels + 0] << 8) | (row[x * channels + 0]);
                }
                d[line + x] = col;
            }
        }

        fclose(fp);
        return img;
    }

    Image *Image::loadJPEG(const std::string &filename)
    {
        jpeg_decompress_struct cinfo;
        ErrorManager jerr;

        FILE *infile = fopen(filename.c_str(), "rb");
        if (!infile)
        {
            return nullptr;
        }

        cinfo.err = jpeg_std_error(&jerr.defaultErrorManager);
        cinfo.out_color_space = JCS_EXT_RGB;
        jerr.defaultErrorManager.error_exit = ErrorExit;
        jerr.defaultErrorManager.output_message = OutputMessage;

        if (setjmp(jerr.jumpBuffer))
        {
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            return nullptr;
        }

        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, infile);
        jpeg_read_header(&cinfo, TRUE);

        if (cinfo.num_components == 3)
        {
            cinfo.out_color_space = JCS_EXT_BGRA;
        }

        jpeg_start_decompress(&cinfo);

        Image *img = new Image(cinfo.image_width, cinfo.image_height, cinfo.num_components);
        uint8_t *d = (uint8_t *)img->get_data();

        while (cinfo.output_scanline < cinfo.image_height)
        {
            uint8_t *p = d + cinfo.output_scanline * cinfo.image_width * sizeof(uint32_t);
            jpeg_read_scanlines(&cinfo, &p, 1);
        }

        img->update_pixels();

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

        return img;
    }

    void Image::save(const std::string &filename)
    {
        this->save(filename, this->dpi);
    }

    void Image::save(const std::string &filename, unsigned int dpi)
    {
        if (access(filename.c_str(), F_OK) == 0)
        {
            remove(filename.c_str());
        }
        return this->savePNG(filename, dpi);
    }

    void Image::savePNG(const std::string &filename, unsigned int dpi)
    {
        FILE *fp = fopen(filename.c_str(), "wb");
        if (!fp)
        {
            throw;
        }

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            fclose(fp);
            throw;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            fclose(fp);
            throw;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            fclose(fp);
            throw;
        }
        png_init_io(png_ptr, fp);

        int color_type = 0;
        switch (this->format)
        {
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
            this->_pixelwidth,
            this->_pixelheight,
            8,
            color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);
        int pixels_per_meter{static_cast<int>((dpi * 100.0) / 2.54)};
        png_set_pHYs(png_ptr, info_ptr, pixels_per_meter, pixels_per_meter, PNG_RESOLUTION_METER);

        // png_write_info(png_ptr, info_ptr);

        png_bytep row_pointers[this->_pixelheight];

        uint8_t *converted_data{nullptr};
        unsigned int stride{this->_pixelwidth};
        switch (this->format)
        {
        case GRAY:
            converted_data = (uint8_t *)::operator new(this->_pixelwidth *this->_pixelheight * sizeof(uint8_t));
            for (unsigned int i = 0; i < this->_pixelwidth * this->_pixelheight; i++)
            {
                converted_data[i] = BLUE(this->data[i]);
            }
            stride = this->_pixelwidth;
            break;
        case RGB:
            converted_data = (uint8_t *)::operator new(this->_pixelwidth *this->_pixelheight * sizeof(uint8_t) * 3);
            for (unsigned int i = 0; i < this->_pixelwidth * this->_pixelheight; i++)
            {
                converted_data[i * 3] = BLUE(this->data[i]);
                converted_data[i * 3 + 1] = GREEN(this->data[i]);
                converted_data[i * 3 + 2] = RED(this->data[i]);
            }
            stride = this->_pixelwidth * 3;
            break;
        case ARGB:
        default:
            converted_data = (uint8_t *)this->get_data();
            stride = this->_pixelwidth * 4;
            break;
        }

        png_bytep data = (png_bytep)converted_data;
        for (unsigned int y = 0; y < this->_pixelheight; y++)
        {
            row_pointers[y] = &(data[y * stride]);
        }

        png_set_rows(png_ptr, info_ptr, row_pointers);
        // png_write_image(png_ptr, row_pointers);
        // png_write_end(png_ptr, NULL);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    // Pixel access functions
    void *Image::get_pre_multiplied_data()
    {
        if (this->cache == nullptr)
        {
            this->cache = malloc(this->_pixelwidth * this->_pixelheight * sizeof(unsigned char) * this->format);
            unsigned char *cache = (unsigned char *)this->cache;
            unsigned char *source = this->load_bytes();

            for (unsigned int i = 0; i < this->_pixelwidth * this->_pixelheight * 4; i += 4)
            {
                cache[i] = source[i] * source[i + 3] / 255;
                cache[i + 1] = source[i + 1] * source[i + 3] / 255;
                cache[i + 2] = source[i + 2] * source[i + 3] / 255;
                cache[i + 3] = source[i + 3];
            }
        }
        return this->cache;
    }

    uint32_t *Image::load_pixels()
    {
        if (this->data == nullptr)
        {
            throw;
        }
        this->pixels_locked = true;

        return (uint32_t *)this->data;
    }

    uint8_t *Image::load_bytes()
    {
        this->pixels_locked = true;
        return (uint8_t *)this->data;
    }

    void Image::update_pixels()
    {
        update_pixels(0, 0, this->_pixelwidth, this->_pixelheight);
    }

    void Image::update_pixels(int x, int y, int w, int h)
    {
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;

        if (w < 0)
            w = 0;
        if (h < 0)
            h = 0;
        int x2 = x + w;
        int y2 = y + h;

        if (!modified)
        {
            mx1 = max(0, x);
            mx2 = x2 < 0 ? _pixelwidth : min(_pixelwidth, (unsigned int)(x2));
            my1 = max(0, y);
            my2 = y2 < 0 ? _pixelheight : min(_pixelheight, (unsigned int)(y2));
            modified = true;
        }
        else
        {
            if (x < mx1)
                mx1 = max(0, x);
            if (x > mx2)
                mx2 = min(_pixelwidth, (unsigned int)(x));
            if (y < my1)
                my1 = max(0, y);
            if (y > my2)
                my2 = min(_pixelheight, (unsigned int)(y));

            if (x2 < mx1)
                mx1 = max(0, x2);
            if (x2 > mx2)
                mx2 = min(_pixelwidth, (unsigned int)(x2));
            if (y2 < my1)
                my1 = max(0, y2);
            if (y2 > my2)
                my2 = min(_pixelheight, (unsigned int)(y2));
        }
    }

    unsigned int Image::operator[](unsigned int index)
    {
        if (index < this->_pixelwidth * this->_pixelheight)
        {
            // return (this->load_pixels())[index];
            return this->data[index];
        }
        throw;
    }

    Image *Image::get()
    {
        return this->get(0, 0, this->_pixelwidth, this->_pixelheight);
    }

    uint32_t Image::get(int x, int y)
    {
        if (x < 0 || y < 0 || static_cast<unsigned int>(x) >= this->_pixelwidth || static_cast<unsigned int>(y) >= this->_pixelheight)
            return 0;

        switch (format)
        {
        case GRAY:
            return data[y * this->_pixelwidth + x] | 0xff;
        case RGB:
            return data[y * this->_pixelwidth + x] | 0xff000000;
        case ARGB:
            return data[y * this->_pixelwidth + x];
        case ALPHA:
            return (data[y * this->_pixelwidth + x] << 24) | 0xffffff;
        }
        throw;
    }

    Image *Image::get(int x, int y, int width, int height)
    {
        Image *out = createimage(width, height, this->format);
        uint32_t *src_data = this->load_pixels();
        uint32_t *dst_data = out->load_pixels();

        for (int ty = 0; ty < height; ty++)
        {
            for (int tx = 0; tx < width; tx++)
            {
                unsigned int pos = (y + ty) * this->_pixelwidth + (x + tx);
                uint32_t pixel = src_data[pos];
                dst_data[ty * width * +tx] = pixel;
            }
        }

        this->update_pixels();
        out->update_pixels();
        return out;
    }

    void Image::set(unsigned int x, unsigned int y, unsigned int c)
    {
        if (x < this->_pixelwidth && y < this->_pixelheight)
        {
            unsigned int *pixels = this->load_pixels();
            pixels[y * this->_pixelwidth + x] = c;
        }
    }

    void Image::set(int x, int y, Image *img)
    {
        unsigned int *pixels = this->load_pixels();
        unsigned int *inpixels = img->load_pixels();

        for (unsigned int sy = 0; sy < img->_pixelheight; sy++)
        {
            int ay = y + sy;
            for (unsigned int sx = 0; sx < img->_pixelwidth; sx++)
            {
                int ax = x + sx;
                if (ax >= 0 && (unsigned int)(ax) < this->_pixelwidth && ay >= 0 && (unsigned int)(ay) < this->_pixelheight)
                {
                    pixels[ay * this->_pixelwidth + ax] = aoverb(inpixels[sy * img->_pixelwidth + sx], pixels[ay * this->_pixelwidth + ax]);
                }
            }
        }
    }

    void Image::set(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int c)
    {
        unsigned int *pixels = this->load_pixels();
        for (unsigned int iy = 0; iy < height; iy++)
        {
            unsigned int ay = y + iy;
            for (unsigned int ix = 0; ix < width; ix++)
            {
                unsigned int ax = x + ix;
                if (ax < this->_pixelwidth && ay < this->_pixelheight)
                {
                    pixels[ay * this->_pixelwidth + ax] = c;
                }
            }
        }
        this->update_pixels();
    }

    /*
     * BLENDING FUNCTIONS
     */

    uint32_t blend_blend(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;
        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + (src & RB_MASK) * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + (src & GN_MASK) * s_a) >> 8 & GN_MASK);
    }

    uint32_t blend_add_pin(uint32_t dst, uint32_t src)
    {
        unsigned int a = ALPHA(src);
        unsigned int s_a = a + (a >= 0x7F ? 1 : 0);

        unsigned int rb = (dst & RB_MASK) + ((src & RB_MASK) * s_a >> 8 & RB_MASK);
        unsigned int gn = (dst & GN_MASK) + ((src & GN_MASK) * s_a >> 8);

        return min((dst >> 24) + a, 0xFFu) << 24 |
               min(rb & 0xFFFF0000, RED_MASK) |
               min(gn & 0x00FFFF00, GREEN_MASK) |
               min(rb & 0x0000FFFF, BLUE_MASK);
    }

    uint32_t blend_sub_pin(uint32_t dst, uint32_t src)
    {
        uint32_t a = ALPHA(src);
        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);

        int s_r = ((RED(src) * s_a) >> 8) & 255;
        int s_g = ((GREEN(src) * s_a) >> 8) & 255;
        int s_b = ((BLUE(src) * s_a) >> 8) & 255;

        int d_r = RED(dst);
        int d_g = GREEN(dst);
        int d_b = BLUE(dst);

        unsigned int o_a = (min(ALPHA(dst) + a, 0xFFu) << 24);
        unsigned int o_r = (max(d_r - s_r, 0) & 255) << 16;
        unsigned int o_g = (max(d_g - s_g, 0) & 255) << 8;
        unsigned int o_b = (max(d_b - s_b, 0) & 255);

        return o_a | o_r | o_g | o_b;
    }

    uint32_t blend_subtract(int a, int b, int factor)
    {
        return std::max(b - (a * factor) / 255, 0);
    }

    /**
     * Darkest
     * O = MIN(D, S)
     */
    uint32_t blend_darkest(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t rb = min(src & RED_MASK, dst & RED_MASK) |
                      min(src & BLUE_MASK, dst & BLUE_MASK);
        uint32_t gn = min(src & GREEN_MASK, dst & GREEN_MASK);

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /**
     * Lightest
     * O = MAX(D, S)
     */
    uint32_t blend_lightest(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t rb = max(src & RED_MASK, dst & RED_MASK) |
                      max(src & BLUE_MASK, dst & BLUE_MASK);
        uint32_t gn = max(src & GREEN_MASK, dst & GREEN_MASK);

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /**
     * Difference
     * O = ABS(D - S)
     */
    uint32_t blend_difference(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t r = abs(int((dst & RED_MASK) - (src & RED_MASK)));
        uint32_t b = abs(int((dst & BLUE_MASK) - (src & BLUE_MASK)));
        uint32_t g = abs(int((dst & GREEN_MASK) - (src & GREEN_MASK)));

        uint32_t rb = (r < 0 ? -r : r) |
                      (b < 0 ? -b : b);
        uint32_t gn = (g < 0 ? -g : g);

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /*
     * Multiply
     * O = DS
     */
    uint32_t blend_multiply(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t d_gn = dst & GN_MASK;

        uint32_t f_r = (dst & RED_MASK) >> 16;
        uint32_t f_b = (dst & BLUE_MASK);

        uint32_t rb =
            ((src & RED_MASK) * (f_r + 1) |
             (src & BLUE_MASK) * (f_b + 1)) >>
                8 &
            RB_MASK;
        uint32_t gn =
            (src & GREEN_MASK) * (d_gn + 0x100) >> 16 & GN_MASK;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               ((d_gn * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /**
     * Screen
     * O = 1 - (1 - D)(1 - S)
     * O = D + S - DS
     */
    uint32_t blend_screen(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t d_rb = dst & RB_MASK;
        uint32_t d_gn = dst & GN_MASK;

        uint32_t s_gn = src & GN_MASK;

        uint32_t f_r = (dst & RED_MASK) >> 16;
        uint32_t f_b = (dst & BLUE_MASK);

        uint32_t rb_sub =
            ((src & RED_MASK) * (f_r + 1) |
             (src & BLUE_MASK) * (f_b + 1)) >>
                8 &
            RB_MASK;
        uint32_t gn_sub = s_gn * (d_gn + 0x100) >> 16 & GN_MASK;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               ((d_rb * d_a + (d_rb + (src & RB_MASK) - rb_sub) * s_a) >> 8 & RB_MASK) |
               ((d_gn * d_a + (d_gn + s_gn - gn_sub) * s_a) >> 8 & GN_MASK);
    }

    /**
     * Overlay
     * O = 2 * MULTIPLY(D, S) = 2DS                   for D < 0.5
     * O = 2 * SCREEN(D, S) - 1 = 2(S + D - DS) - 1   otherwise
     */
    uint32_t blend_overlay(uint32_t dst, uint32_t src)
    {
        uint32_t a = src >> 24;

        uint32_t s_a = a + (a >= 0x7F ? 1 : 0);
        uint32_t d_a = 0x100 - s_a;

        uint32_t d_r = dst & RED_MASK;
        uint32_t d_g = dst & GREEN_MASK;
        uint32_t d_b = dst & BLUE_MASK;

        uint32_t s_r = src & RED_MASK;
        uint32_t s_g = src & GREEN_MASK;
        uint32_t s_b = src & BLUE_MASK;

        uint32_t r = (d_r < 0x800000) ? d_r * ((s_r >> 16) + 1) >> 7 : 0xFF0000 - ((0x100 - (s_r >> 16)) * (RED_MASK - d_r) >> 7);
        uint32_t g = (d_g < 0x8000) ? d_g * (s_g + 0x100) >> 15 : (0xFF00 - ((0x10000 - s_g) * (GREEN_MASK - d_g) >> 15));
        uint32_t b = (d_b < 0x80) ? d_b * (s_b + 1) >> 7 : (0xFF00 - ((0x100 - s_b) * (BLUE_MASK - d_b) << 1)) >> 8;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + ((r | b) & RB_MASK) * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + (g & GN_MASK) * s_a) >> 8 & GN_MASK);
    }

    /**
     * Exclusion
     * O = (1 - S)D + S(1 - D)
     * O = D + S - 2DS
     */
    uint32_t blend_exclusion(uint32_t dst, uint32_t src)
    {
        int a = src >> 24;

        int s_a = a + (a >= 0x7F ? 1 : 0);
        int d_a = 0x100 - s_a;

        int d_rb = dst & RB_MASK;
        int d_gn = dst & GN_MASK;

        int s_gn = src & GN_MASK;

        int f_r = (dst & RED_MASK) >> 16;
        int f_b = (dst & BLUE_MASK);

        int rb_sub =
            ((src & RED_MASK) * (f_r + (f_r >= 0x7F ? 1 : 0)) |
             (src & BLUE_MASK) * (f_b + (f_b >= 0x7F ? 1 : 0))) >>
                7 &
            0x01FF01FF;
        int gn_sub = s_gn * (d_gn + (d_gn >= 0x7F00 ? 0x100 : 0)) >> 15 & 0x0001FF00;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               ((d_rb * d_a + (d_rb + (src & RB_MASK) - rb_sub) * s_a) >> 8 & RB_MASK) |
               ((d_gn * d_a + (d_gn + s_gn - gn_sub) * s_a) >> 8 & GN_MASK);
    }

    /**
     * Hard Light
     * O = OVERLAY(S, D)
     *
     * O = 2 * MULTIPLY(D, S) = 2DS                   for S < 0.5
     * O = 2 * SCREEN(D, S) - 1 = 2(S + D - DS) - 1   otherwise
     */
    uint32_t blend_hard_light(uint32_t dst, uint32_t src)
    {
        int a = src >> 24;

        int s_a = a + (a >= 0x7F ? 1 : 0);
        int d_a = 0x100 - s_a;

        int d_r = dst & RED_MASK;
        int d_g = dst & GREEN_MASK;
        int d_b = dst & BLUE_MASK;

        int s_r = src & RED_MASK;
        int s_g = src & GREEN_MASK;
        int s_b = src & BLUE_MASK;

        int r = (s_r < 0x800000) ? s_r * ((d_r >> 16) + 1) >> 7 : 0xFF0000 - ((0x100 - (d_r >> 16)) * (RED_MASK - s_r) >> 7);
        int g = (s_g < 0x8000) ? s_g * (d_g + 0x100) >> 15 : (0xFF00 - ((0x10000 - d_g) * (GREEN_MASK - s_g) >> 15));
        int b = (s_b < 0x80) ? s_b * (d_b + 1) >> 7 : (0xFF00 - ((0x100 - d_b) * (BLUE_MASK - s_b) << 1)) >> 8;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + ((r | b) & RB_MASK) * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + (g & GN_MASK) * s_a) >> 8 & GN_MASK);
    }

    /**
     * Soft Light (Pegtop)
     * O = (1 - D) * MULTIPLY(D, S) + D * SCREEN(D, S)
     * O = (1 - D) * DS + D * (1 - (1 - D)(1 - S))
     * O = 2DS + DD - 2DDS
     */
    uint32_t blend_soft_light(uint32_t dst, uint32_t src)
    {
        int a = src >> 24;

        int s_a = a + (a >= 0x7F ? 1 : 0);
        int d_a = 0x100 - s_a;

        int d_r = dst & RED_MASK;
        int d_g = dst & GREEN_MASK;
        int d_b = dst & BLUE_MASK;

        int s_r1 = src & RED_MASK >> 16;
        int s_g1 = src & GREEN_MASK >> 8;
        int s_b1 = src & BLUE_MASK;

        int d_r1 = (d_r >> 16) + (s_r1 < 0x7F ? 1 : 0);
        int d_g1 = (d_g >> 8) + (s_g1 < 0x7F ? 1 : 0);
        int d_b1 = d_b + (s_b1 < 0x7F ? 1 : 0);

        int r = ((s_r1 * d_r >> 7) + 0xFF * d_r1 * (d_r1 + 1) -
                 ((s_r1 * d_r1 * d_r1) << 1)) &
                RED_MASK;
        int g = ((s_g1 * d_g << 1) + 0xFF * d_g1 * (d_g1 + 1) -
                 ((s_g1 * d_g1 * d_g1) << 1)) >>
                    8 &
                GREEN_MASK;
        int b = ((s_b1 * d_b << 9) + 0xFF * d_b1 * (d_b1 + 1) -
                 ((s_b1 * d_b1 * d_b1) << 1)) >>
                16;

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + (r | b) * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + g * s_a) >> 8 & GN_MASK);
    }

    /**
     * Dodge
     * O = D / (1 - S)
     */
    uint32_t blend_dodge(uint32_t dst, uint32_t src)
    {
        int a = src >> 24;

        int s_a = a + (a >= 0x7F ? 1 : 0);
        int d_a = 0x100 - s_a;

        int r = (dst & RED_MASK) / (256 - ((src & RED_MASK) >> 16));
        int g = ((dst & GREEN_MASK) << 8) / (256 - ((src & GREEN_MASK) >> 8));
        int b = ((dst & BLUE_MASK) << 8) / (256 - (src & BLUE_MASK));

        int rb =
            (r > 0xFF00 ? 0xFF0000 : ((r << 8) & RED_MASK)) |
            (b > 0x00FF ? 0x0000FF : b);
        int gn =
            (g > 0xFF00 ? 0x00FF00 : (g & GREEN_MASK));

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /**
     * Burn
     * O = 1 - (1 - A) / B
     */
    uint32_t blend_burn(uint32_t dst, uint32_t src)
    {
        int a = src >> 24;

        int s_a = a + (a >= 0x7F ? 1 : 0);
        int d_a = 0x100 - s_a;

        int r = ((0xFF0000 - (dst & RED_MASK))) / (1 + (src & RED_MASK >> 16));
        int g = ((0x00FF00 - (dst & GREEN_MASK)) << 8) / (1 + (src & GREEN_MASK >> 8));
        int b = ((0x0000FF - (dst & BLUE_MASK)) << 8) / (1 + (src & BLUE_MASK));

        int rb = RB_MASK -
                 (r > 0xFF00 ? 0xFF0000 : ((r << 8) & RED_MASK)) -
                 (b > 0x00FF ? 0x0000FF : b);
        int gn = GN_MASK -
                 (g > 0xFF00 ? 0x00FF00 : (g & GREEN_MASK));

        return min((dst >> 24) + a, 0xFFu) << 24 |
               (((dst & RB_MASK) * d_a + rb * s_a) >> 8 & RB_MASK) |
               (((dst & GN_MASK) * d_a + gn * s_a) >> 8 & GN_MASK);
    }

    /* End BLEND FUNCTIONS */

    void Image::blend(Image *src,
                      int sx, int sy, unsigned int sw, unsigned int sh,
                      int dx, int dy, unsigned int dw, unsigned int dh, BLENDMODE mode)
    {

        int sx2 = sx + sw;
        int sy2 = sy + sh;
        int dx2 = dx + dw;
        int dy2 = dy + dh;

        if (src == this)
        {
            if (intersect(sx, sy, sx2, sy2, dx, dy, dx2, dy2))
            {
                blit_resize(get(sx, sy, sw, sh),
                            0, 0, sw, sh,
                            (uint32_t *)this->get_data(), this->_pixelwidth, this->_pixelheight, dx, dy, dx2, dy2, mode);
            }
            else
            {
                blit_resize(src, sx, sy, sx2, sy2,
                            (uint32_t *)this->get_data(), this->_pixelwidth, this->_pixelheight,
                            dx, dy, dx2, dy2, mode);
            }
        }
        else
        {
            blit_resize(src, sx, sy, sx2, sy2,
                        this->load_pixels(), this->_pixelwidth, this->_pixelheight,
                        dx, dy, dx2, dy2, mode);
        }
        this->update_pixels();
    }

    bool Image::intersect(int sx1, int sy1, int sx2, int sy2, int dx1, int dy1, int dx2, int dy2)
    {
        int sw = sx2 - sx1 + 1;
        int sh = sy2 - sy1 + 1;
        int dw = dx2 - dx1 + 1;
        int dh = dy2 - dy1 + 1;

        if (dx1 < sx1)
        {
            dw += dx1 - sx1;
            if (dw > sw)
            {
                dw = sw;
            }
        }
        else
        {
            int w = sw + sx1 - dx1;
            if (dw > w)
            {
                dw = w;
            }
        }
        if (dy1 < sy1)
        {
            dh += dy1 - sy1;
            if (dh > sh)
            {
                dh = sh;
            }
        }
        else
        {
            int h = sh + sy1 - dy1;
            if (dh > h)
            {
                dh = h;
            }
        }
        return !(dw <= 0 || dh <= 0);
    }

    void Image::blit_resize(Image *img,
                            int srcX1, int srcY1, int srcX2, int srcY2,
                            uint32_t *destpixels, int screenW, int screenH,
                            int destX1, int destY1, int destX2, int destY2,
                            unsigned int mode)
    {

        srcX1 = max(0, srcX1);
        srcY1 = max(0, srcY1);
        srcX2 = srcX2 < 0 ? 0 : min(img->_pixelwidth, (unsigned int)(srcX2));
        srcY2 = srcY2 < 0 ? 0 : min(img->_pixelheight, (unsigned int)(srcY2));

        int srcW{srcX2 - srcX1};
        int srcH{srcY2 - srcY1};
        int destW{destX2 - destX1};
        int destH{destY2 - destY1};

        bool smooth{true};

        if (!smooth)
        {
            srcW++;
            srcH++;
        }

        if (destW <= 0 || destH <= 0 ||
            srcW <= 0 || srcH <= 0 ||
            (destX1 > 0 && (destX1) >= screenW) || (destY1 > 0 && (destY1) >= screenH) ||
            (unsigned int)(srcX1) >= img->_pixelwidth || (unsigned int)(srcY1) >= img->_pixelheight)
        {
            return;
        }

        float dx{(srcW / (float)destW * PRECISIONF)};
        float dy{(srcH / (float)destH * PRECISIONF)};

        if (destX1 < 0)
        {
            destW += destX1;
            destX1 = 0;
        }

        if (destY1 < 0)
        {
            destH += destY1;
            destY1 = 0;
        }

        destW = min(destW, screenW - destX1);
        destH = min(destH, screenH - destY1);

        int64_t destoffset{destY1 * screenW + destX1};
        uint32_t *srcbuffer{img->load_pixels()};

        float srcxoffset{destX1 < 0 ? -destX1 * dx : srcX1 * PRECISIONF};
        float srcyoffset{destY1 < 0 ? -destY1 * dy : srcY1 * PRECISIONF};

        int64_t fracU, ifU, fracV, ifV, u1, u2, v1{0}, v2{0}, sX, iw, iw1, ih1;
        int64_t ul, ll, ur, lr;

        uint32_t cUL, cLL, cUR, cLR;
        uint32_t r, g, b, a;

        auto filter_new_scanline = [&]()
        {
            sX = srcxoffset;
            fracV = static_cast<unsigned int>(srcyoffset) & PREC_MAXVAL;
            ifV = PREC_MAXVAL - fracV + 1;
            v1 = (static_cast<unsigned int>(srcyoffset) >> PRECISIONB) * iw;
            v2 = min((static_cast<unsigned int>(srcyoffset) >> PRECISIONB) + 1, ih1) * iw;
        };

        auto filter_bilinear = [&]()
        {
            fracU = sX & PREC_MAXVAL;
            ifU = PREC_MAXVAL - fracU + 1;
            ul = (ifU * ifV) >> PRECISIONB;
            ll = ifU - ul;
            ur = ifV - ul;
            lr = PREC_MAXVAL + 1 - ul - ll - ur;
            u1 = (sX >> PRECISIONB);
            u2 = min(u1 + 1, iw1);

            // get color values of the 4 neighbouring texels
            cUL = srcbuffer[v1 + u1];
            cUR = srcbuffer[v1 + u2];
            cLL = srcbuffer[v2 + u1];
            cLR = srcbuffer[v2 + u2];

            r = ((ul * ((cUL & RED_MASK) >> 16) + ll * ((cLL & RED_MASK) >> 16) +
                  ur * ((cUR & RED_MASK) >> 16) + lr * ((cLR & RED_MASK) >> 16))
                 << PREC_RED_SHIFT) &
                RED_MASK;

            g = ((ul * (cUL & GREEN_MASK) + ll * (cLL & GREEN_MASK) +
                  ur * (cUR & GREEN_MASK) + lr * (cLR & GREEN_MASK)) >>
                 PRECISIONB) &
                GREEN_MASK;

            b = (ul * (cUL & BLUE_MASK) + ll * (cLL & BLUE_MASK) +
                 ur * (cUR & BLUE_MASK) + lr * (cLR & BLUE_MASK)) >>
                PRECISIONB;

            a = ((ul * ((cUL & ALPHA_MASK) >> 24) + ll * ((cLL & ALPHA_MASK) >> 24) +
                  ur * ((cUR & ALPHA_MASK) >> 24) + lr * ((cLR & ALPHA_MASK) >> 24))
                 << PREC_ALPHA_SHIFT) &
                ALPHA_MASK;

            return a | r | g | b;
        };

        uint32_t (*blend_function)(uint32_t dst, uint32_t src);

        switch (mode)
        {
        case BLENDMODE::EXCLUSION:
            blend_function = {blend_exclusion};
            break;
        case BLENDMODE::HARD_LIGHT:
            blend_function = {blend_hard_light};
            break;
        case BLENDMODE::SOFT_LIGHT:
            blend_function = {blend_soft_light};
            break;
        case BLENDMODE::DODGE:
            blend_function = {blend_dodge};
            break;
        case BLENDMODE::BURN:
            blend_function = {blend_burn};
            break;
        case BLENDMODE::OVERLAY:
            blend_function = {blend_overlay};
            break;
        case BLENDMODE::SCREEN:
            blend_function = {blend_screen};
            break;
        case BLENDMODE::MULTIPLY:
            blend_function = {blend_multiply};
            break;
        case BLENDMODE::DIFFERENCE:
            blend_function = {blend_difference};
            break;
        case BLENDMODE::LIGHTEST:
            blend_function = {blend_lightest};
            break;
        case BLENDMODE::DARKEST:
            blend_function = {blend_darkest};
            break;
        case BLENDMODE::SUBTRACT:
            blend_function = {blend_sub_pin};
            break;
        case BLENDMODE::ADD:
            blend_function = {blend_add_pin};
            break;
        case BLENDMODE::BLEND:
        default:
            blend_function = {blend_blend};
            break;
        }

        if (smooth)
        {
            iw = img->_pixelwidth;
            iw1 = img->_pixelwidth - 1;
            ih1 = img->_pixelheight - 1;

            for (int y{0}; y < destH; y++)
            {
                filter_new_scanline();
                for (int x{0}; x < destW; x++)
                {
                    destpixels[destoffset + x] = blend_function(destpixels[destoffset + x],
                                                                filter_bilinear());
                    sX += dx;
                }
                destoffset += screenW;
                srcyoffset += dy;
            }
        }
        else
        {
        }
    }

    void Image::mask(Image *mask)
    {
        if (this->format == 1 || this->format == 3)
        {
            throw;
        }
        if (this->_pixelwidth != mask->_pixelwidth || this->_pixelheight != mask->_pixelheight)
        {
            throw;
        }
        uint32_t *mask_data = mask->load_pixels();
        uint32_t *data = this->load_pixels();

        for (unsigned int i = 0; i < this->_pixelwidth * this->_pixelheight; i++)
        {
            data[i] = (data[i] & 0xffffff) | (BLUE(mask_data[i]) << 24);
        }

        mask->update_pixels();
        this->update_pixels();
    }

    Image *Image::convert(unsigned int format)
    {
        if (this->format == format)
        {
            return this->get();
        }

        Image *out = createimage(this->_pixelwidth, this->_pixelheight, format);
        uint32_t *out_p = out->load_pixels();
        uint32_t *in_p = this->load_pixels();

        for (unsigned int i = 0; i < this->_pixelwidth * this->_pixelheight; i++)
        {
            if (this->format == GRAY)
            {
                if (format == ARGB || format == RGB)
                {
                    out_p[i] = (in_p[i] << 16) | (in_p[i] << 8) | (in_p[i]) | 0xff000000;
                }
            }
            else
            {
                if (format == ARGB)
                {
                    out_p[i] = in_p[i];
                }
                else if (format == RGB)
                {
                    out_p[i] = in_p[i] | 0xff000000;
                }
                else if (format == ALPHA)
                {
                    out_p[i] = in_p[i] & 0xff;
                }
                else if (format == GRAY)
                {
                    out_p[i] = RED(in_p[i]) * 0.2126 + GREEN(in_p[i]) * 0.7152 + BLUE(in_p[i]) * 0.0722;
                }
            }
        }

        this->update_pixels();
        out->update_pixels();

        return out;
    }

    Image *Image::resize(unsigned int width, unsigned int height, RESIZEMETHOD method)
    {
        if (width == 0)
        {
            width = uint((float)this->_pixelwidth / ((float)this->_pixelheight / (float)height));
        }
        else if (height == 0)
        {
            height = uint((float)this->_pixelheight / ((float)this->_pixelwidth / (float)width));
        }
        switch (method)
        {
        case BILINEAR:
            return this->resize_bilinear(width, height);
        case NEAREST:
        default:
            return this->resize_nearest(width, height);
        }
    }

    Image *Image::resize_nearest(unsigned int width, unsigned int height)
    {
        float sx = float(width) / float(this->_pixelwidth);
        float sy = float(height) / float(this->_pixelheight);

        Image *out = createimage(width, height, this->format);

        uint32_t *out_pixels = out->load_pixels();
        uint32_t *in_pixels = this->load_pixels();

        for (unsigned int oy = 0; oy < out->_pixelheight; oy++)
        {
            unsigned int out_line = oy * out->_pixelwidth;
            unsigned int in_line = int(oy / sy) * this->_pixelwidth;

            for (unsigned int ox = 0; ox < out->_pixelwidth; ox++)
            {
                unsigned int in_col = int(float(ox) / sx);
                out_pixels[out_line + ox] = in_pixels[in_line + in_col];
            }
        }
        this->update_pixels();
        out->update_pixels();

        return out;
    }

    Image *Image::resize_bilinear(unsigned int width, unsigned int height)
    {
        float sx = (float)width / (float)this->_pixelwidth;
        float sy = (float)height / (float)this->_pixelheight;

        Image *out = createimage(width, height, this->format);

        unsigned char *out_pixels = (unsigned char *)out->get_data();
        unsigned char *in_pixels = (unsigned char *)this->get_data();

        for (unsigned int y = 0; y < height; y++)
        {
            float in_y = (float)y / (float)sy;
            unsigned int y1 = (uint32_t)in_y;
            unsigned int y2 = (uint32_t)in_y + 1;
            float y_lerp = in_y - y1;
            for (unsigned int x = 0; x < width; x++)
            {
                float in_x = (float)x / (float)sx;
                unsigned int x1 = (uint32_t)in_x;
                unsigned int x2 = (uint32_t)in_x + 1;
                float x_lerp = in_x - x1;

                for (unsigned int ch = 0; ch < 4; ch++)
                {
                    unsigned char q11 = in_pixels[y1 * this->_pixelwidth * 4 + x1 * 4 + ch];
                    unsigned char q12 = in_pixels[y2 * this->_pixelwidth * 4 + x1 * 4 + ch];
                    unsigned char q21 = in_pixels[y1 * this->_pixelwidth * 4 + x2 * 4 + ch];
                    unsigned char q22 = in_pixels[y2 * this->_pixelwidth * 4 + x2 * 4 + ch];

                    unsigned char xy1 = lerp(q11, q21, x_lerp);
                    unsigned char xy2 = lerp(q12, q22, x_lerp);
                    unsigned char out_color = lerp(xy1, xy2, y_lerp);
                    out_pixels[y * width * 4 + x * 4 + ch] = out_color;
                }
            }
        }

        out->update_pixels();
        this->update_pixels();
        return out;
    }

    uint32_t lerp_color(uint32_t a, uint32_t b, float t)
    {
        return lerp(ALPHA(a), ALPHA(b), t) << 24 |
               lerp(RED(a), RED(b), t) << 16 |
               lerp(GREEN(a), GREEN(b), t) << 8 |
               lerp(BLUE(a), BLUE(b), t);
    }

    Image *Image::rotate(double angle, RESIZEMETHOD method)
    {
        switch (method)
        {
        case RESIZEMETHOD::NEAREST:
            return this->rotate_nearest(angle);
            break;
        case RESIZEMETHOD::BILINEAR:
        default:
            return this->rotate_bilinear(angle);
            break;
        }
    }

    Image *Image::rotate_nearest(double angle)
    {
        double ca = cos(-angle);
        double sa = sin(-angle);
        int new_width = ceil(abs(this->_pixelwidth * sin(angle)) + abs(this->_pixelheight * cos(angle))) + 1;
        int new_height = ceil(abs(this->_pixelwidth * cos(angle)) + abs(this->_pixelheight * sin(angle))) + 1;

        float hw = (new_width - 1) / 2.0;
        float hh = (new_height - 1) / 2.0;

        Image *out = new Image(new_width, new_height, ARGB);
        uint32_t *pixels = out->load_pixels();
        for (unsigned int y = 0; y < out->_pixelheight; y++)
        {
            for (unsigned int x = 0; x < out->_pixelwidth; x++)
            {
                int sx = round(((x - hw) * ca - (y - hh) * sa) + hw);
                int sy = round(((x - hw) * sa + (y - hh) * ca) + hh);

                pixels[y * out->_pixelwidth + x] = this->get(sx, sy);
            }
        }

        out->update_pixels();

        return out;
    }

    Image *Image::rotate_bilinear(double angle)
    {
        double ca = cos(-angle);
        double sa = sin(-angle);
        int new_width = round(std::abs(this->_pixelwidth * sin(angle)) + std::abs(this->_pixelheight * cos(angle)));
        int new_height = round(std::abs(this->_pixelwidth * cos(angle)) + std::abs(this->_pixelheight * sin(angle)));

        float hw = (new_width - 1) / 2.0;
        float hh = (new_height - 1) / 2.0;

        Image *out = new Image(new_width, new_height, ARGB);
        uint32_t *pixels = out->load_pixels();
        for (unsigned int y = 0; y < out->_pixelheight; y++)
        {
            for (unsigned int x = 0; x < out->_pixelwidth; x++)
            {
                float sx = ((x - hw) * ca - (y - hh) * sa) + hw;
                float sy = ((x - hw) * sa + (y - hh) * ca) + hh;

                int y1 = floor(sy);
                int y2 = ceil(sy);
                float y_lerp = sy - y1;

                int x1 = floor(sx);
                int x2 = ceil(sx);
                float x_lerp = sx - x1;

                pixels[y * out->_pixelwidth + x] = lerp_color(
                    lerp_color(this->get(x1, y1), this->get(x2, y1), x_lerp),
                    lerp_color(this->get(x1, y2), this->get(x2, y2), x_lerp),
                    y_lerp);
                // pixels[y * out->_pixelwidth + x] = this->get(x1, y1);
            }
        }

        out->update_pixels();

        return out;
    }

    // Utility functions
    Image *createimage(unsigned int width, unsigned int height, int format, unsigned int dpi, Unit unit)
    {
        return new Image(width, height, format, 1, dpi, unit);
    }
} // namespace Pyro
