#include <pyro/pyroconstants.h>
#include <pyro/pyrocolor.h>

namespace Pyro {
    void Color::set(float a, float b, float c, float d) {
        switch(this->mode) {
            case Pyro::HSB:
            case Pyro::HSBA:
                this->hsba(a, b, c, d);
                break;

            case Pyro::RGB:
            case Pyro::RGBA:
            default:
                this->r = a;
                this->g = b;
                this->b = c;
                this->a = d;
                break;

        }
    }
    void Color::hsba(float hue, float saturation, float brightness, float a) {
        float C = brightness * saturation;
        float Hp = hue * 6.0f;
        float X = C * (1.0f - std::abs(fmod(Hp, 2.0f) - 1.0f));
        float R1 = 0.0f;
        float G1 = 0.0f;
        float B1 = 0.0f;

        if(Hp <= 1) {
            R1 = C;
            G1 = X;
        } else if(Hp <= 2) {
            R1 = X;
            G1 = C;
        } else if(Hp <= 3) {
            G1 = C;
            B1 = X;
        } else if(Hp <= 4) {
            G1 = X;
            B1 = C;
        } else if(Hp <= 5) {
            R1 = X;
            B1 = C;
        } else if(Hp <= 6) {
            R1 = C;
            B1 = X;
        }

        float m = brightness - C;

        this->r = R1 + m;
        this->g = G1 + m;
        this->b = B1 + m;
        this->a = a;
    }

    Palette::Palette() : colors(std::vector<Color>()) {
    }

    unsigned int Palette::add(Color col) {
        this->colors.push_back(col);
        return this->colors.size() - 1;
    }

    Color Palette::get(unsigned int index) {
        return this->colors[index];
    }

    unsigned int Palette::size() {
        return this->colors.size();
    }

    Color Palette::operator[] (const unsigned int index) {
        return this->colors[index];
    }
}