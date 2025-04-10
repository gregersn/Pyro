#ifndef PYROGRAPHICS_CAIRO_H
#define PYROGRAPHICS_CAIRO_H

#include "graphics.h"

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

namespace Pyro
{
    class GraphicsCairo : public Graphics
    {
      private:
        cairo_surface_t *surface{nullptr};
        cairo_t *cr{nullptr};

        cairo_font_face_t *font{nullptr};

      public:
        GraphicsCairo(unsigned int width, unsigned int height,
                      GraphicsMode mode = GraphicsMode::CAIRO,
                      std::filesystem::path filename = "");
        ~GraphicsCairo() override;
        void init() override;

        void blendmode(int mode) override;

        void translate(float x, float) override;
        void rotate(float a) override;
        void scale(float sx, float sy) override;
        void pushmatrix() override;
        void popmatrix() override;

        void popstyle() override;

        void image_impl(Image *img, float x, float y) override;
        void shape(Shape s, float x, float y) override;
        void line(float x0, float y0, float x1, float y1) override;

        void background(float r, float g, float b, float a) override;
        void smooth() override;
        void smooth(int level) override;
        void nosmooth() override;
        void strokecap(int cap) override;
        void strokejoin(int join) override;

        void text_impl(std::string text, float x, float y) override;
        void textfont_impl(Font *font) override;
    };
} // namespace Pyro

#endif
