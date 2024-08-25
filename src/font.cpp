#include "pyro/font.h"
#include "pyro/font_impl.h"

namespace Pyro
{
    Font::Font(std::filesystem::path filename) : filename(filename)
    {
        this->impl = new FontImpl(filename);
    }

    Font *Font::load(std::filesystem::path filename)
    {
        Font *font{new Font(filename)};
        return font;
    }

    Font *create_font(std::filesystem::path filename, int size)
    {
        // TODO: Use size
        return Font::load(filename);
    }
}
