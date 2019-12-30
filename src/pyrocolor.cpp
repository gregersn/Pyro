#include "pyrocolor.h"

namespace Pyro {
    void Color::set(float a, float b, float c, float d) {
        switch(this->mode) {
            case ColorMode::HSB:
            case ColorMode::HSBA:
                this->hsba(a, b, c, d);
                break;

            case ColorMode::RGB:
            case ColorMode::RGBA:
            default:
                this->r = a;
                this->g = b;
                this->b = c;
                this->a = d;
                break;

        }
    }
    void Color::hsba(float h, float s, float b, float a) {

        float C = b * s;
        float Hp = h * 6.0;
        float X = C * (1.0f - abs(fmod(Hp, 2) - 1));
        float R1 = 0.0f;
        float G1 = 0.0f;
        float B1 = 0.0f;

        if(Hp <= 1.0f) {
            R1 = C;
            G1 = X;
        } else if(Hp <= 2.0f) {
            R1 = X;
            G1 = C;
        } else if(Hp <= 3.0f) {
            G1 = C;
            B1 = X;
        } else if(Hp <= 4.0f) {
            G1 = X;
            B1 = C;
        } else if(Hp <= 5.0f) {
            R1 = C;
            B1 = X;
        } else if(Hp <= 6.0f) {
            R1 = X;
            B1 = C;
        }

        float m = b - C;

        this->r = R1 + m;
        this->g = G1 + m;
        this->b = B1 + m;
        this->a = a;
    }

    Palette::Palette() {
         this->colors = std::vector<Color>();
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