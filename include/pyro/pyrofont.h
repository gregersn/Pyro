#ifndef PYROFONT_H
#define PYROFONT_H
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Pyro {
    class Font {
        private:
            std::string filename = "";
            FT_Face face = nullptr;

        public:
            Font(std::string filename);
            static Font * load(std::string filename);
            FT_Face get_ft_face() { return this->face; };
    };

    Font * create_font(std::string filename, int size);
}
#endif
