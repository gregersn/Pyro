#define _USE_MATH_DEFINES
#include <cmath>

#include "pyro/graphics.h"
#include "pyro/graphics_cairo.h"
#include "pyro/utils.h"

#include <iostream>
#include <cstring>

namespace Pyro
{
    Graphics::Graphics(unsigned int width, unsigned int height, unsigned int format, unsigned int dpi) : Image(width, height, format, 1, dpi)
    {
        this->smooth();
        this->fill(1.0f, 1.0f, 1.0f, 1.0f);
        this->stroke(0.0f, 0.0f, 0.0f, 1.0f);
        this->strokeweight(1.0f);
        this->textsize(16.0f);
        this->strokecap(ROUND);
        this->background(192);
    }

    Graphics::~Graphics()
    {
    }

    Graphics *Graphics::create(unsigned int width, unsigned int height, GraphicsMode mode, unsigned int dpi, Unit unit)
    {

        width = unit2pixels(width, unit, dpi);
        height = unit2pixels(height, unit, dpi);
        switch (mode)
        {
        case GraphicsMode::CAIRO:
        default:
            return new GraphicsCairo(width, height, ARGB, dpi);
        }
    }

    void Graphics::point(float x, float y, Unit unit)
    {
        x = unit2pixels(x, unit, this->get_dpi());
        y = unit2pixels(y, unit, this->get_dpi());

        this->line(x, y, x + 1, y + 1);
    }

    // Coordinates
    void Graphics::translate(float x, float y, Unit unit)
    {
        x = unit2pixels(x, unit, this->get_dpi());
        y = unit2pixels(y, unit, this->get_dpi());

        this->transformer.translate(x, y);
    }

    void Graphics::rotate(float a)
    {
        this->transformer.rotate(a);
    }

    void Graphics::scale(float sx, float sy)
    {
        this->transformer.scale(sx, sy);
    }

    void Graphics::pushmatrix()
    {
        this->transformer.pushmatrix();
    }

    void Graphics::popmatrix()
    {
        this->transformer.popmatrix();
    }

    void Graphics::endshape(int close)
    {
        this->_shape.end(close);
        this->shape(this->_shape, 0, 0);
    }

    void Graphics::background(float r, float g, float b, float a)
    {
        Color c = Color(r, g, b, a);
        char *buf = (char *)this->get_data();
        union
        {
            uint8_t d8[4];
            uint32_t d32;
        } u32 = {.d32 = c.to_uint()};

        size_t count = this->_pixelwidth * this->_pixelheight;
        while (count--)
        {
            *buf++ = u32.d8[0];
            *buf++ = u32.d8[1];
            *buf++ = u32.d8[2];
            *buf++ = u32.d8[3];
        }
    }

    void Graphics::image(Image *img, float x, float y)
    {
        if (this->_image_mode == CENTER)
        {
            x -= img->width() / 2.0f;
            y -= img->height() / 2.0f;
        }

        this->image_impl(img, x, y);
    };

    void Graphics::triangle(Vector a, Vector b, Vector c)
    {
        Shape s = Shape();
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::triangle(float x0, float y0, float x1, float y1, float x2, float y2, Unit unit)
    {
        x0 = unit2pixels(x0, unit, this->get_dpi());
        y0 = unit2pixels(y0, unit, this->get_dpi());
        x1 = unit2pixels(x1, unit, this->get_dpi());
        y1 = unit2pixels(y1, unit, this->get_dpi());
        x2 = unit2pixels(x2, unit, this->get_dpi());
        y2 = unit2pixels(y2, unit, this->get_dpi());

        Shape s = Shape();
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::rect(float a, float b, float c, float d, Unit unit)
    {
        a = unit2pixels(a, unit, this->get_dpi());
        b = unit2pixels(b, unit, this->get_dpi());
        c = unit2pixels(c, unit, this->get_dpi());
        d = unit2pixels(d, unit, this->get_dpi());

        if (this->_rect_mode == CENTER)
        {
            a -= c / 2.0f;
            b -= d / 2.0f;
        }

        Shape s = Shape();
        s.begin();
        s.vertex(a, b);
        s.vertex(a + c, b);
        s.vertex(a + c, b + d);
        s.vertex(a, b + d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit)
    {
        x0 = unit2pixels(x0, unit, this->get_dpi());
        y0 = unit2pixels(y0, unit, this->get_dpi());
        x1 = unit2pixels(x1, unit, this->get_dpi());
        y1 = unit2pixels(y1, unit, this->get_dpi());
        x2 = unit2pixels(x2, unit, this->get_dpi());
        y2 = unit2pixels(y2, unit, this->get_dpi());
        x3 = unit2pixels(x3, unit, this->get_dpi());
        y3 = unit2pixels(y3, unit, this->get_dpi());

        Shape s = Shape();
        s.begin();
        s.curvevertex(x0, y0);
        s.curvevertex(x1, y1);
        s.curvevertex(x2, y2);
        s.curvevertex(x3, y3);
        s.end(OPEN);
        this->shape(s, 0, 0);
    }

    void Graphics::bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit)
    {
        x0 = unit2pixels(x0, unit, this->get_dpi());
        y0 = unit2pixels(y0, unit, this->get_dpi());
        x1 = unit2pixels(x1, unit, this->get_dpi());
        y1 = unit2pixels(y1, unit, this->get_dpi());
        x2 = unit2pixels(x2, unit, this->get_dpi());
        y2 = unit2pixels(y2, unit, this->get_dpi());
        x3 = unit2pixels(x3, unit, this->get_dpi());
        y3 = unit2pixels(y3, unit, this->get_dpi());

        Shape s = Shape();
        s.begin();
        s.vertex(x0, y0);
        s.beziervertex(x1, y1, x2, y2, x3, y3);
        s.end(OPEN);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(Vector a, Vector b, Vector c, Vector d)
    {
        Shape s = Shape();
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.vertex(d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, Unit unit)
    {
        x0 = unit2pixels(x0, unit, this->get_dpi());
        y0 = unit2pixels(y0, unit, this->get_dpi());
        x1 = unit2pixels(x1, unit, this->get_dpi());
        y1 = unit2pixels(y1, unit, this->get_dpi());
        x2 = unit2pixels(x2, unit, this->get_dpi());
        y2 = unit2pixels(y2, unit, this->get_dpi());
        x3 = unit2pixels(x3, unit, this->get_dpi());
        y3 = unit2pixels(y3, unit, this->get_dpi());

        Shape s = Shape();
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.vertex(x3, y3);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::arc(float x, float y,
                       float w, float h,
                       float start, float end, int mode, Unit unit)
    {
        x = unit2pixels(x, unit, this->get_dpi());
        y = unit2pixels(y, unit, this->get_dpi());
        w = unit2pixels(w, unit, this->get_dpi());
        h = unit2pixels(h, unit, this->get_dpi());

        Shape s = Shape();
        s.begin();
        if (end < start)
        {
            end += TWO_PI;
        }
        int steps = 32;
        float da = (end - start) / (float)steps;
        float a = start;
        for (int i = 0; i < steps + 1; i++)
        {
            a = start + i * da;
            s.vertex(cos(a) * w / 2.0f, sin(a) * h / 2.0f);
        }
        if (mode == PIE)
            s.vertex(0, 0);

        if (mode == PIE || mode == CHORD)
            s.end(CLOSE);
        else
            s.end(OPEN);

        this->shape(s, x, y);
    }

    void Graphics::ellipse(float x, float y, float w, float h, unsigned int segments, Unit unit)
    {
        Shape s = Shape();
        s.begin();
        float da = M_PI / (segments / 2.0f);
        x = unit2pixels(x, unit, this->get_dpi());
        y = unit2pixels(y, unit, this->get_dpi());
        w = unit2pixels(w, unit, this->get_dpi());
        h = unit2pixels(h, unit, this->get_dpi());
        for (unsigned int i = 0; i < segments; i++)
        {
            s.vertex(cos(i * da) * w / 2.0f + x,
                     sin(i * da) * h / 2.0f + y);
        }
        s.end(CLOSE);

        this->shape(s, 0, 0);
    }

    void Graphics::stroke(float r, float g, float b, float a)
    {
        this->stroke_color.set(r, g, b, a);
        this->stroke_enable = true;
    }

    void Graphics::stroke(int r, int g, int b, int a)
    {
        this->stroke(r / 255.0f,
                     g / 255.0f,
                     b / 255.0f,
                     a / 255.0f);
    }

    void Graphics::stroke(int c, int a)
    {
        this->stroke(c, c, c, a);
    }

    void Graphics::fill(float r, float g, float b, float a)
    {
        this->fill_color.set(r, g, b, a);
        this->fill_enable = true;
    }

    void Graphics::fill(int r, int g, int b, int a)
    {
        this->fill(r / 255.0f,
                   g / 255.0f,
                   b / 255.0f,
                   a / 255.0f);
    }

    void Graphics::fill(int c, int a)
    {
        this->fill(c, c, c, a);
    }

    void Graphics::nostroke()
    {
        this->stroke_enable = false;
    }

    void Graphics::nofill()
    {
        this->fill_enable = false;
    }

    void Graphics::smooth()
    {
        this->_smooth = true;
    }

    void Graphics::nosmooth()
    {
        this->_smooth = false;
    }

    void Graphics::strokeweight(float w, Unit unit)
    {
        unsigned int dpi = this->get_dpi();
        this->stroke_weight = unit2pixels(w, unit, dpi);
    }

    void Graphics::strokecap(int cap)
    {
    }

    void Graphics::strokejoin(int join)
    {
    }

    // Typography

    void Graphics::textsize(float size, Unit unit)
    {
        this->text_size = size;
    }

    void Graphics::textfont(Font *font)
    {
        this->textfont_impl(font);
    }

    void Graphics::text(std::string text, float x, float y, Unit unit)
    {
        this->text_impl(text, unit2pixels(x, unit, this->get_dpi()), unit2pixels(y, unit, this->get_dpi()));
    }
}