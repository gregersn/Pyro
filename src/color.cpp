#include "pyro/constants.h"
#include "pyro/color.h"

namespace Pyro
{
    void Color::set(float a, float b, float c, float d)
    {
        switch (this->mode)
        {
        case Pyro::HSB:
        case Pyro::HSBA:
            this->hsba(a, b, c, d);
            break;

        case Pyro::RGB:
        case Pyro::RGBA:
        default:
            this->_red = a;
            this->_green = b;
            this->_blue = c;
            this->_alpha = d;
            break;
        }
    }
    void Color::hsba(float hue, float saturation, float brightness, float a)
    {
        float C = brightness * saturation;
        float Hp = hue * 6.0f;
        float X = C * (1.0f - std::abs(fmod(Hp, 2.0f) - 1.0f));
        float R1 = 0.0f;
        float G1 = 0.0f;
        float B1 = 0.0f;

        if (Hp <= 1)
        {
            R1 = C;
            G1 = X;
        }
        else if (Hp <= 2)
        {
            R1 = X;
            G1 = C;
        }
        else if (Hp <= 3)
        {
            G1 = C;
            B1 = X;
        }
        else if (Hp <= 4)
        {
            G1 = X;
            B1 = C;
        }
        else if (Hp <= 5)
        {
            R1 = X;
            B1 = C;
        }
        else if (Hp <= 6)
        {
            R1 = C;
            B1 = X;
        }

        float m = brightness - C;

        this->_red = R1 + m;
        this->_green = G1 + m;
        this->_blue = B1 + m;
        this->_alpha = a;
    }

    void Color::update_hsl()
    {
        if (this->dirty_hsl)
        {
            float Xmax = max(max(this->_red, this->_green), this->_blue);
            float Xmin = min(min(this->_red, this->_green), this->_blue);

            if (Xmax == Xmin)
            {
                this->_hue = 0.0f;
                this->_saturation = 0.0f;
                this->_lightness = this->_red;
            }
            else
            {
                this->_lightness = (Xmax + Xmin) / 2.0f;
                if (this->_lightness < 0.5f)
                {
                    this->_saturation = (Xmax - Xmin) / (Xmax + Xmin);
                }
                else
                {
                    this->_saturation = (Xmax - Xmin) / (2.0f - Xmax - Xmin);
                }

                if (this->_red == Xmax)
                    this->_hue = (this->_green - this->_blue) / (Xmax - Xmin);
                else if (this->_green == Xmax)
                    this->_hue = 2.0 + (this->_blue - this->_red) / (Xmax - Xmin);
                else
                    this->_hue = 4.0 + (this->_red - this->_green) / (Xmax - Xmin);

                this->_hue /= 6.0f;

                this->dirty_hsl = false;
            }
        }
    }

    void Color::update_rgb()
    {
        if (this->dirty_rgb)
        {
            if (this->_saturation == 0.0)
            {
                this->_red = this->_lightness;
                this->_green = this->_lightness;
                this->_blue = this->_lightness;
            }
            else
            {
                float temp2;
                if (this->_lightness < 0.5)
                    temp2 = this->_lightness * (1.0 + this->_saturation);
                else
                    temp2 = (this->_lightness + this->_saturation) - (this->_lightness * this->_saturation);

                float temp1;
                temp1 = 2.0 * this->_lightness - temp2;
                float tempr = this->_hue + 1.0 / 3.0;
                if (tempr > 1)
                    tempr--;
                float tempg = this->_hue;
                float tempb = this->_hue - 1.0 / 3.0;
                if (tempb < 0)
                    tempb++;

                if (tempr < 1.0 / 6.0)
                    this->_red = temp1 + (temp2 - temp1) * 6.0 * tempr;
                else if (tempr < 0.5)
                    this->_red = temp2;
                else if (tempr < 2.0 / 3.0)
                    this->_red = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
                else
                    this->_red = temp1;

                if (tempg < 1.0 / 6.0)
                    this->_green = temp1 + (temp2 - temp1) * 6.0 * tempg;
                else if (tempg < 0.5)
                    this->_green = temp2;
                else if (tempg < 2.0 / 3.0)
                    this->_green = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
                else
                    this->_green = temp1;

                if (tempb < 1.0 / 6.0)
                    this->_blue = temp1 + (temp2 - temp1) * 6.0 * tempb;
                else if (tempb < 0.5)
                    this->_blue = temp2;
                else if (tempb < 2.0 / 3.0)
                    this->_blue = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
                else
                    this->_blue = temp1;
            }

            this->dirty_rgb = false;
        }
    }

    float Color::lightness()
    {
        this->update_hsl();
        return this->_lightness;
    }

    float Color::hue()
    {
        this->update_hsl();
        return this->_hue;
    }

    float Color::saturation()
    {
        this->update_hsl();
        return this->_saturation;
    }

    float Color::r()
    {
        this->update_rgb();
        return this->_red;
    }

    float Color::g()
    {
        this->update_rgb();
        return this->_green;
    }

    float Color::b()
    {
        this->update_rgb();
        return this->_blue;
    }

    float Color::a()
    {
        return this->_alpha;
    }

    void Color::lightness(float b)
    {
        if (this->dirty_hsl && !this->dirty_rgb)
        {
            this->update_hsl();
        }
        this->_lightness = b;
        this->dirty_rgb = true;
    }

    Palette::Palette() : colors(std::vector<Color>())
    {
    }

    unsigned int Palette::add(Color col)
    {
        this->colors.push_back(col);
        return this->colors.size() - 1;
    }

    Color Palette::get(unsigned int index)
    {
        return this->colors[index];
    }

    unsigned int Palette::size()
    {
        return this->colors.size();
    }

    Color Palette::operator[](const unsigned int index)
    {
        return this->colors[index];
    }

    uint32_t color(unsigned int gray)
    {
        return color(gray, 255);
    }

    uint32_t color(unsigned int gray, unsigned int alpha)
    {
        return color(gray, gray, gray, alpha);
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b)
    {
        return 0xff000000 | r << 16 | g << 8 | b;
    }

    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
    {
        return a << 24 | r << 16 | g << 8 | b;
    }
}