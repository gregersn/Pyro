#include <pyro/pyrographics_gsn.h>

namespace Pyro
{
    GraphicsGSN::GraphicsGSN(unsigned int width, unsigned int height, unsigned int format, unsigned int dpi) : Graphics(width, height, format, 1)
    {
    }

    GraphicsGSN::~GraphicsGSN()
    {
    }

}