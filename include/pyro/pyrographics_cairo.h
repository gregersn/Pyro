#ifndef PYROGRAPHICS_CAIRO_H
#define PYROGRAPHICS_CAIRO_H

#include "pyrographics.h"

#include <cairo/cairo.h>


namespace Pyro {
    class GraphicsCairo : public Graphics {
        cairo_surface_t *surface;
        cairo_t *cr;


        public:
            GraphicsCairo(unsigned int width, unsigned int height, unsigned int channels);
            ~GraphicsCairo();

            void blendmode(BlendMode mode);

            void translate(float x, float);
            void rotate(float a);
            void pushmatrix();
            void popmatrix();

            void image(Image *img, float x, float y);
            void shape(Shape s, float x, float y);
            void line(float x0, float y0, float x1, float y1);
            void ellipse(float x, float y, float w, float h, unsigned int segments);
            void background(float r, float g, float b, float a);
            void smooth();
            void nosmooth();
            void strokecap(int cap);

    };
}

#endif
