#include <ft2build.h>
#include FT_FREETYPE_H
#include <cairo-ft.h>

#include <pyro/pyrofont_cairo.h>

namespace Pyro {
    FT_Library FontCairo::ftlib;
    Font * Pyro::FontCairo::load(std::string filename) {
        FontCairo * font = new FontCairo();
        FT_Error status;

        status = FT_Init_FreeType(&ftlib);
        if(status != 0) {
            // Fail
            printf("Could not init freetype\n");
            nullptr;
        }

        status = FT_New_Face(ftlib, filename.c_str(), 0, &font->face);
        if(status != 0) {
            // Fail
            printf("Could not create new face\n");
            return nullptr;
        }

        font->ct = cairo_ft_font_face_create_for_ft_face(font->face, 0);

        return font;
    }
}
