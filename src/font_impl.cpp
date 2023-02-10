#include "pyro/font_impl.h"

namespace Pyro
{
    FontImpl::FontImpl(std::string filename) : filename(filename)
    {
        FT_Library library;
        FT_Error error{FT_Init_FreeType(&library)};
        if (error)
        {
        }

        error = FT_New_Face(library,
                            filename.c_str(),
                            0,
                            &face);
        if (error == FT_Err_Unknown_File_Format)
        {
            /*... the font file could be opened and read, but it appears
            ... that its font format is unsupported*/
        }
        else if (error)
        {
            /*... another error code means that the font file could not
            ... be opened or read, or that it is broken...*/
        }
    }
}
