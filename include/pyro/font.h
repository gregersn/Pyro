#ifndef PYROFONT_H
#define PYROFONT_H
#include <filesystem>

namespace Pyro
{
    class FontImpl;

    class Font
    {
    private:
        std::filesystem::path filename{""};

    public:
        explicit Font(std::filesystem::path filename);
        static Font *load(std::filesystem::path filename);
        FontImpl *impl;
    };

    Font *create_font(std::filesystem::path filename, int size);
}
#endif
