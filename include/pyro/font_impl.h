#ifndef PYROFONT_IMPL_H
#define PYROFONT_IMPL_H
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Pyro
{
    class FontImpl
    {
    private:
        std::string filename = "";
        FT_Face face = nullptr;

    public:
        FontImpl(std::string filename);
        FT_Face get_ft_face() { return this->face; };
    };
}
#endif
