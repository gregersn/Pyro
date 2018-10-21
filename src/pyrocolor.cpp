#include "pyrocolor.h"

namespace Pyro {
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