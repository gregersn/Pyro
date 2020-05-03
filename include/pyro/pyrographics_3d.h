#ifndef PYROGRAPHICS_3D_H
#define PYROGRAPHICS_3D_H

#include "pyrographics.h"


namespace Pyro {
    class Graphics3d : public Graphics {
        public:
            Graphics3d(const Graphics3d &in);
            Graphics3d(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
            ~Graphics3d();

            Graphics3d &operator=(const Graphics3d &in);

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

            void text_impl(std::string text, float x, float y);
    };
}

#endif
