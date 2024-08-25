#ifndef PYROFONT_IMPL_H
#define PYROFONT_IMPL_H
#include <filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Pyro
{
    class FontImpl
    {
    private:
        std::filesystem::path filename{""};
        FT_Face face{nullptr};

    public:
        explicit FontImpl(std::filesystem::path filename);
        FT_Face get_ft_face() { return this->face; };
    };
}
#endif
