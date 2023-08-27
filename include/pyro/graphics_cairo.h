#ifndef PYROGRAPHICS_CAIRO_H
#define PYROGRAPHICS_CAIRO_H

#include "graphics.h"

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

namespace Pyro
{
    class GraphicsCairo : public Graphics
    {
        cairo_surface_t *surface{nullptr};
        cairo_t *cr{nullptr};

        cairo_font_face_t *font{nullptr};

    public:
        GraphicsCairo(const GraphicsCairo &in);
        GraphicsCairo(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi, Unit unit);
        ~GraphicsCairo();

        GraphicsCairo &operator=(const GraphicsCairo &in);

        void blendmode(BlendMode mode) override;

        void translate(float x, float, Unit unit = Unit::current) override;
        void rotate(float a) override;
        void scale(float sx, float sy) override;
        void pushmatrix() override;
        void popmatrix() override;

        void image_impl(Image *img, float x, float y) override;
        void shape(Shape s, float x, float y, Unit unit = Unit::current) override;
        void line(float x0, float y0, float x1, float y1, Unit unit = Unit::current) override;
        void background(float r, float g, float b, float a) override;
        void smooth() override;
        void nosmooth() override;
        void strokecap(int cap) override;
        void strokejoin(int join) override;

        void text_impl(std::string text, float x, float y, Unit unit = Unit::current) override;
        void textfont_impl(Font *font) override;
    };
}

#endif
