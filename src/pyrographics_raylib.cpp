#include "pyro/pyrographics_raylib.h"
#include "pyro/pyrofont_impl.h"

#include <iostream>
namespace RayLib
{
#include "raylib.h"
}

namespace Pyro
{
    GraphicsRaylib::GraphicsRaylib(unsigned int width, unsigned int height, unsigned int format, unsigned int dpi) : Graphics(width, height, format, 1)
    {
    }

    GraphicsRaylib::~GraphicsRaylib()
    {
    }

    void GraphicsRaylib::background(float r, float g, float b, float a)
    {
        RayLib::ClearBackground((RayLib::Color){uint8_t(r * 255), uint8_t(g * 255), uint8_t(b * 255), uint8_t(a * 255)});
    }
    void GraphicsRaylib::line(float x0, float y0, float x1, float y1)
    {
        Pyro::Color *stroke = &this->stroke_color;
        RayLib::DrawLine(x0, y0, x1, y1, (RayLib::Color){uint8_t(stroke->r * 255), uint8_t(stroke->g * 255), uint8_t(stroke->b * 255), uint8_t(stroke->a * 255)});
    }
}