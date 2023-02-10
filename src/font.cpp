#include "pyro/font.h"
#include "pyro/font_impl.h"

namespace Pyro
{
    Font::Font(std::string filename) : filename(filename)
    {
        this->impl = new FontImpl(filename);
    }

    Font *Font::load(std::string filename)
    {
        Font *font{new Font(filename)};
        return font;
    }

    Font *create_font(std::string filename, int size)
    {
        return Font::load(filename);
    }
}
