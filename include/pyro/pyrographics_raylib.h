#ifndef PYROGRAPHICS_RAYLIB_H
#define PYROGRAPHICS_RAYLIB_H

#include "pyrographics.h"

namespace Pyro
{
    class GraphicsRaylib : public Graphics
    {
    public:
        GraphicsRaylib(const GraphicsRaylib &in);
        GraphicsRaylib(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
        ~GraphicsRaylib();

        GraphicsRaylib &operator=(const GraphicsRaylib &in);

        void background(float r, float g, float b, float a);
        void line(float x0, float y0, float x1, float y1);

        /*void blendmode(BlendMode mode);

        void translate(float x, float);
        void rotate(float a);
        void scale(float sx, float sy);
        void pushmatrix();
        void popmatrix();

        void image_impl(Image *img, float x, float y);
        void shape(Shape s, float x, float y);
        void ellipse(float x, float y, float w, float h, unsigned int segments);
        void smooth();
        void nosmooth();
        void strokecap(int cap);
        void strokejoin(int join);

        void text_impl(std::string text, float x, float y);
        void textfont_impl(Font *font);*/
    };
}

#endif
