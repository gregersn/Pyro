#ifndef PYROFONT_H
#define PYROFONT_H
#include <string>

namespace Pyro
{
    class FontImpl;

    class Font
    {
    private:
        std::string filename{""};

    public:
        explicit Font(std::string filename);
        static Font *load(std::string filename);
        FontImpl *impl;
    };

    Font *create_font(std::string filename, int size);
}
#endif
