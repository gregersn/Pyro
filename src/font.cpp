#include "pyro/font.h"
#include "pyro/font_impl.h"

namespace Pyro
{
    Font::Font(std::string const filename) : filename(filename)
    {
        this->impl = new FontImpl(filename);
    }

    Font *Font::load(std::string const filename)
    {
        Font *font{new Font(filename)};
        return font;
    }

    Font *create_font(std::string filename, int size)
    {
        return Font::load(filename);
    }
}
