#ifndef PYROCOLOR_H
#define PYROCOLOR_H

#include <string>
#include <vector>

#include "constants.h"
#include "math.h"

namespace Pyro
{
    class Color
    {
    private:
        float _hue{0.0f};
        float _saturation{0.0f};
        float _lightness{0.0f};
        float _red{0.0f};
        float _green{0.0f};
        float _blue{0.0f};
        float _alpha{0.0f};

        bool dirty_hsl{true};
        bool dirty_rgb{false};

        void update_hsl();
        void update_rgb();

    public:
        float r();
        float g();
        float b();
        float a();

        int mode{Pyro::RGBA};

        void colormode(int mode)
        {
            this->mode = mode;
        }

        Color()
        {
        }

        explicit Color(std::string const &color)
        {
            if (color.length() == 7)
            {
                this->_alpha = 1.0f;
                long unsigned int t_c{std::stoul(color.substr(1, 7), nullptr, 16)};
                this->_red = ((t_c & 0xff0000) >> 16) / 255.0f;
                this->_green = ((t_c & 0xff00) >> 8) / 255.0f;
                this->_blue = ((t_c & 0xff)) / 255.0f;
            }
        }
        explicit Color(float v)
        {
            this->_red = v;
            this->_green = v;
            this->_blue = v;
            this->_alpha = 1.0f;
        }
        Color(float v, float a)
        {
            this->_red = v;
            this->_green = v;
            this->_blue = v;
            this->_alpha = a;
        }
        Color(float r, float g, float b)
        {
            this->_red = r;
            this->_green = g;
            this->_blue = b;
            this->_alpha = 1.0f;
        }

        Color(float r, float g, float b, float a)
        {
            this->_red = r;
            this->_green = g;
            this->_blue = b;
            this->_alpha = a;
        }

        Color(unsigned int r, unsigned int g, unsigned int b)
        {
            this->_red = r / 255.0f;
            this->_green = g / 255.0f;
            this->_blue = b / 255.0f;
            this->_alpha = 1.0f;
        }

        Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
        {
            this->_red = r / 255.0f;
            this->_green = g / 255.0f;
            this->_blue = b / 255.0f;
            this->_alpha = a / 255.0f;
        }

        static Color from_uint(uint32_t c)
        {
            return Color(
                (c & 0xff0000) >> 16,
                (c & 0xff00) >> 8,
                (c & 0xff),
                (c & 0xff000000) >> 24);
        }

        // HSB is the same as HSV
        void hsba(float h, float s, float b, float a);

        static Color hsla(float h, float s, float l, float a);

        uint32_t to_uint()
        {
            return (
                (uint8_t)(this->_alpha * 255) << 24 |
                (uint8_t)(this->_red * 255) << 16 |
                (uint8_t)(this->_green * 255) << 8 |
                (uint8_t)(this->_blue * 255));
        }

        static uint32_t fto32(float r, float g, float b, float a)
        {
            return (
                (uint8_t)(a * 255) << 24 |
                (uint8_t)(r * 255) << 16 |
                (uint8_t)(g * 255) << 8 |
                (uint8_t)(b * 255));
        }

        Color lerp(Color other, float t)
        {
            return Color(Pyro::lerp(this->_red, other.r(), t),
                         Pyro::lerp(this->_green, other.g(), t),
                         Pyro::lerp(this->_blue, other.b(), t),
                         Pyro::lerp(this->_alpha, other.a(), t));
        }

        void set(float a, float b, float c, float d);

        float hue();
        float saturation();
        float lightness();

        void lightness(float l);

        bool operator==(const Color &rhs) const;
    };

    class Palette
    {
        std::vector<Pyro::Color> colors;

    public:
        Palette();
        unsigned int add(Color col);
        Color get(unsigned int index);
        unsigned int size();

        Color operator[](const unsigned int index);
    };

    /* TODO:

    alpha
    blue
    brightness
    */
    uint32_t color(unsigned int gray, unsigned int alpha = 255);
    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
    /* TODO:

    green
    hue
    lerpcolor
    red
    saturation

    */

}

#endif
