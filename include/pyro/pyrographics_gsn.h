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

        void shape(Shape s, float x, float y);
        void line(float x0, float y0, float x1, float y1);
    };
}

#endif