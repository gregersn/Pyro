#include "pyrofont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cairo/cairo.h>

namespace Pyro {
    class FontCairo : public Font {
        static FT_Library ftlib;
    public:
        static Font * load(std::string filename);
        FT_Face face;
        cairo_font_face_t *ct;
    };
}