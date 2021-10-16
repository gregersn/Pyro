#ifndef PYROGRAPHICS_CAIRO_H
#define PYROGRAPHICS_CAIRO_H

#include "graphics.h"

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

namespace Pyro
{
    class GraphicsCairo : public Graphics
    {
        cairo_surface_t *surface = nullptr;
        cairo_t *cr = nullptr;

        cairo_font_face_t *font = nullptr;

    public:
        GraphicsCairo(const GraphicsCairo &in);
        GraphicsCairo(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
        ~GraphicsCairo();

        GraphicsCairo &operator=(const GraphicsCairo &in);

        void blendmode(BlendMode mode);

        void translate(float x, float);
        void rotate(float a);
        void scale(float sx, float sy);
        void pushmatrix();
        void popmatrix();

        void image_impl(Image *img, float x, float y);
        void shape(Shape s, float x, float y);
        void line(float x0, float y0, float x1, float y1);
        void ellipse(float x, float y, float w, float h, unsigned int segments);
        void background(float r, float g, float b, float a);
        void smooth();
        void nosmooth();
        void strokecap(int cap);
        void strokejoin(int join);

        void text_impl(std::string text, float x, float y);
        void textfont_impl(Font *font);
    };
}

#endif
