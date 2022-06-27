#pragma once

#include "graphics.h"

namespace Pyro
{
    class GraphicsPyro3D : public Graphics
    {
    public:
        GraphicsPyro3D(const GraphicsPyro3D &in);
        GraphicsPyro3D(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
        ~GraphicsPyro3D();

        GraphicsPyro3D &operator=(const GraphicsPyro3D &in);

        void point(float x0, float y0);
        void line(float x0, float y0, float x1, float y1, Unit unit = Unit::current);
    };
}