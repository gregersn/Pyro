#ifndef PYROGRAPHICS_2D_H
#define PYROGRAPHICS_2D_H

#include "pyrographics.h"
#include "pyrotransformer.h"

namespace Pyro {
    class Graphics2D : public Graphics {
        private:
            uint32_t color_pack(float r, float g, float b, float a);
            Transformer2D transformer;            

            void draw_line(float x0, float y0, float x1, float y1);

        public:
            Graphics2D(const Graphics2D &in);
            Graphics2D(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
            ~Graphics2D();

            Graphics2D &operator=(const Graphics2D &in);

            // Coordinates
            void translate(float x, float);
            void rotate(float a);
            void scale(float sx, float sy);
            void pushmatrix();
            void popmatrix();

            // Shapes
            void background(float r, float g, float b, float a);
            void line(float x0, float y0, float x1, float y1);
            void shape(Shape s, float x, float y);
    };
}

#endif
