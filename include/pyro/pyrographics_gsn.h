#ifndef PYROGRAPHICS_GSN_H
#define PYROGRAPHICS_GSN_H

#include "pyrographics.h"

namespace Pyro
{
    class GraphicsGSN : public Graphics
    {
    public:
        GraphicsGSN(const GraphicsGSN &in);
        GraphicsGSN(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
        ~GraphicsGSN();
    };
}

#endif