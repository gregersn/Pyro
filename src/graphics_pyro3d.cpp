#include "pyro/graphics_pyro3d.h"

namespace Pyro
{

    float interpolate(float i0, float d0, float i1, float d1)
    {
        if (i0 == i1)
        {
            return {d0};
        }
    }

    GraphicsPyro3D::GraphicsPyro3D(unsigned int width, unsigned int height, unsigned int format, unsigned int dpi) : Graphics(width, height, format, dpi)
    {
    }

    GraphicsPyro3D::~GraphicsPyro3D()
    {
    }

    void GraphicsPyro3D::point(float x, float y)
    {
        this->set(int(x), int(y), this->stroke_color.to_uint());
    }

    void GraphicsPyro3D::line(float x0, float y0, float x1, float y1, Unit unit)
    {
        if (this->stroke_enable)
        {
            if (abs(x1 - x0) > (y1 - y0))
            {
                // Horizontal-ish
            }
            else
            {
                // Vertical-ish
            }
            if (x0 > x1)
            {
                float t = x1;
                x1 = x0;
                x0 = t;
                t = y1;
                y1 = y0;
                y0 = t;
            }

            float a = (y1 - y0) / (x1 - x0);
            float y = y0;
            for (unsigned int x = int(x0); x <= x1; x++)
            {
                this->set(x, int(y), this->stroke_color.to_uint());
                y += a;
            }
        }
    }

}