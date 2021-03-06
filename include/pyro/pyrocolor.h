#ifndef PYROCOLOR_H
#define PYROCOLOR_H

#include <string>
#include <vector>

#include <pyro/pyroconstants.h>
#include <pyro/pyromath.h>

namespace Pyro
{
    class Color
    {
    public:
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;

        int mode = Pyro::RGBA;

        void colormode(int mode)
        {
            this->mode = mode;
        }

        Color()
        {
        }

        Color(const std::string color)
        {
            if (color.length() == 7)
            {
                this->a = 1.0f;
                unsigned int t_c = std::stoul(color.substr(1, 7), nullptr, 16);
                this->r = ((t_c & 0xff0000) >> 16) / 255.0f;
                this->g = ((t_c & 0xff00) >> 8) / 255.0f;
                this->b = ((t_c & 0xff)) / 255.0f;
            }
        }
        Color(float v)
        {
            this->r = v;
            this->g = v;
            this->b = v;
            this->a = 1.0f;
        }
        Color(float v, float a)
        {
            this->r = v;
            this->g = v;
            this->b = v;
            this->a = a;
        }
        Color(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 1.0f;
        }

        Color(float r, float g, float b, float a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        Color(unsigned int r, unsigned int g, unsigned int b)
        {
            this->r = r / 255.0f;
            this->g = g / 255.0f;
            this->b = b / 255.0f;
            this->a = 1.0f;
        }

        Color(uint r, uint g, uint b, uint a)
        {
            this->r = r / 255.0f;
            this->g = g / 255.0f;
            this->b = b / 255.0f;
            this->a = a / 255.0f;
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
                (uint)(this->a * 255) << 24 |
                (uint)(this->r * 255) << 16 |
                (uint)(this->g * 255) << 8 |
                (uint)(this->b * 255));
        }

        static uint32_t fto32(float r, float g, float b, float a)
        {
            return (
                (uint)(a * 255) << 24 |
                (uint)(r * 255) << 16 |
                (uint)(g * 255) << 8 |
                (uint)(b * 255));
        }

        Color lerp(Color other, float t)
        {
            return Color(Pyro::lerp(this->r, other.r, t),
                         Pyro::lerp(this->g, other.g, t),
                         Pyro::lerp(this->b, other.b, t),
                         Pyro::lerp(this->a, other.a, t));
        }

        void set(float a, float b, float c, float d);
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

    uint32_t color(unsigned int gray);
    uint32_t color(unsigned int gray, unsigned int alpha);
    uint32_t color(unsigned int r, unsigned int g, unsigned int b);
    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

}

#endif