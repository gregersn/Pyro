#include "pyro/graphics_sdl.h"
#include <stdexcept>
#define _USE_MATH_DEFINES
#include <cmath>

#include "pyro/graphics.h"
#include "pyro/graphics_cairo.h"
#include "pyro/graphics_sdl.h"
#include "pyro/utils.h"

#include <iostream>
#include <cstring>

namespace Pyro
{
    Graphics::Graphics(unsigned int width, unsigned int height, std::filesystem::path filename) : Image(width, height, ARGB)
    {
        this->mode = CAIRO;
        this->filename = filename;
    }

    Graphics::~Graphics()
    {
    }

    void Graphics::init()
    {
        this->smooth();
        this->fill(1.0f, 1.0f, 1.0f, 1.0f);
        this->stroke(0.0f, 0.0f, 0.0f, 1.0f);
        this->strokeweight(1.0f);
        this->textsize(16.0f);
        this->strokecap(ROUND);
        this->background(192);
    }

    Graphics *creategraphics(unsigned int width, unsigned int height, GraphicsMode mode, std::filesystem::path filename)
    {

        Graphics *g = nullptr;
        switch (mode)
        {
        case GraphicsMode::SVG:
        case GraphicsMode::PDF:
            assert(filename != "");
            g = new GraphicsCairo(width, height, mode, filename);
            break;
        case GraphicsMode::CAIRO:
            g = new GraphicsCairo(width, height);
            break;
        case GraphicsMode::SDL:
            g = new GraphicsSDL(width, height);
            break;
        default:
            throw std::logic_error("Unknown graphics mode");
            break;
        }
        assert(g != nullptr);
        g->init();
        return g;
    }

    Graphics *creategraphics(unsigned int width, unsigned int height, int dpi, Unit unit, GraphicsMode mode, std::filesystem::path filename)
    {
        Graphics *g = creategraphics(width, height, mode, filename);
        g->set_dpi(dpi);
        g->set_unit(unit);
        g->init();
        return g;
    }

    void Graphics::curvedetail(unsigned int segments)
    {
        this->_curvedetail = segments;
    }

    void Graphics::point(Vector p)
    {
        this->line(p, p + Vector(1.0f, 1.0f));
    }

    void Graphics::point(float x, float y)
    {
        this->line(x, y, x + 1, y + 1);
    }

    // Coordinates
    void Graphics::translate(float x, float y)
    {
        this->transformer.translate(x, y);
    }

    void Graphics::translate(Vector v)
    {
        this->transformer.translate(v);
    }

    void Graphics::rotate(float a)
    {
        this->transformer.rotate(a);
    }

    void Graphics::scale(Vector v)
    {
        this->transformer.scale(v);
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

    void Graphics::pushstyle()
    {
        /*
        The style information controlled by the following functions are included in the style:
        fill(), stroke(), tint(), strokeWeight(), strokeCap(),strokeJoin(),
        imageMode(), rectMode(), ellipseMode(), shapeMode(), colorMode(),
        textAlign(), textFont(), textMode(), textSize(), textLeading(),
        emissive(), specular(), shininess(), ambient()
        */
        this->style.push();
    }

    void Graphics::popstyle()
    {
        this->style.pop();
    }

    float Graphics::screen_x(float x, float y, float z)
    {
        return this->transformer.screen_x(x, y, z);
    }
    float Graphics::screen_y(float x, float y, float z)
    {
        return this->transformer.screen_y(x, y, z);
    }

    void Graphics::endshape(int close)
    {
        this->_shape.end(close);
        this->shape(this->_shape, 0, 0);
    }

    void Graphics::background(float r, float g, float b, float a)
    {
        Color c{Color(r, g, b, a)};
        uint32_t *buf{static_cast<uint32_t *>(this->get_data())};
        uint32_t _color = c.to_uint();

        size_t count{this->_pixelwidth * this->_pixelheight};
        while (count--)
        {
            *buf++ = _color;
        }
    }

    void Graphics::image(Image *img, float x, float y)
    {
        if (this->style.imagemode() == CENTER)
        {
            x -= img->width() / 2.0f;
            y -= img->height() / 2.0f;
        }

        this->image_impl(img, x, y);
    };

    void Graphics::triangle(Vector a, Vector b, Vector c)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::triangle(float x0, float y0, float x1, float y1, float x2, float y2)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::rect(float a, float b, float c, float d)
    {
        if (this->style.rectmode() == CENTER)
        {
            a -= c / 2.0f;
            b -= d / 2.0f;
        }

        Shape s{Shape()};
        s.begin();
        s.vertex(a, b);
        s.vertex(a + c, b);
        s.vertex(a + c, b + d);
        s.vertex(a, b + d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::rect(Vector p0, Vector p1)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(p0);
        s.vertex(p1.x, p0.y);
        s.vertex(p1);
        s.vertex(p0.x, p1.y);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
    {
        curve(Vector(x0, y0), Vector(x1, y1), Vector(x2, y2), Vector(x3, y3));
    }
    void Graphics::curve(Vector p0, Vector p1, Vector p2, Vector p3)
    {
        Shape s{Shape()};
        s.begin();
        s.curvevertex(p0);
        s.curvevertex(p1);
        s.curvevertex(p2);
        s.curvevertex(p3);
        s.end(OPEN);
        this->shape(s, 0, 0);
    }

    void Graphics::bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
    {
        bezier(Vector(x0, y0), Vector(x1, y1), Vector(x2, y2), Vector(x3, y3));
    }

    void Graphics::bezier(Vector p0, Vector p1, Vector p2, Vector p3)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(p0);
        s.beziervertex(p1, p2, p3);
        s.end(OPEN);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(Vector a, Vector b, Vector c, Vector d)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(a);
        s.vertex(b);
        s.vertex(c);
        s.vertex(d);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
    {
        Shape s{Shape()};
        s.begin();
        s.vertex(x0, y0);
        s.vertex(x1, y1);
        s.vertex(x2, y2);
        s.vertex(x3, y3);
        s.end(CLOSE);
        this->shape(s, 0, 0);
    }

    void Graphics::arc(Vector p0, Vector p1, float start, float end, int mode)
    {
        arc(p0.x, p0.y, p1.x, p1.y, start, end, mode);
    }

    void Graphics::arc(float x, float y,
                       float w, float h,
                       float start, float end, int mode)
    {
        Shape s{Shape()};
        s.begin();
        if (end < start)
        {
            end += TWO_PI;
        }
        int steps{32};
        float da{(end - start) / (float)steps};
        for (int i{0}; i < steps + 1; i++)
        {
            float a = start + i * da;
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

    void Graphics::ellipse(Vector p0, float w, float h)
    {
        ellipse(p0.x, p0.y, w, h);
    }

    void Graphics::ellipse(float x, float y, float w, float h)
    {
        Shape s = Shape();
        s.begin();
        float da = M_PI / (this->_curvedetail / 2.0f);

        for (unsigned int i = 0; i < this->_curvedetail; i++)
        {
            s.vertex(cos(i * da) * w / 2.0f,
                     sin(i * da) * h / 2.0f);
        }
        s.end(CLOSE);

        this->shape(s, x, y);
    }

    Color Graphics::stroke(float r, float g, float b, float a)
    {
        return this->style.stroke(r, g, b, a);
    }

    Color Graphics::stroke(int r, int g, int b, int a)
    {
        return this->stroke(r / 255.0f,
                            g / 255.0f,
                            b / 255.0f,
                            a / 255.0f);
    }

    Color Graphics::stroke(int c, int a)
    {
        return this->stroke(c, c, c, a);
    }

    Color Graphics::fill(float r, float g, float b, float a)
    {
        return this->style.fill(r, g, b, a);
    }

    Color Graphics::fill(int r, int g, int b, int a)
    {
        return this->fill(r / 255.0f,
                          g / 255.0f,
                          b / 255.0f,
                          a / 255.0f);
    }

    Color Graphics::fill(int c, int a)
    {
        return this->fill(c, c, c, a);
    }

    void Graphics::nostroke()
    {
        this->style.nostroke();
    }

    void Graphics::nofill()
    {
        this->style.nofill();
    }

    void Graphics::smooth()
    {
        this->smooth(2);
    }

    void Graphics::smooth(int level)
    {
        this->_smooth = max(min(level, 8), 0);
    }

    void Graphics::nosmooth()
    {
        this->smooth(0);
    }

    float Graphics::strokeweight()
    {
        return this->style.strokeweight();
    }

    float Graphics::strokeweight(float w)
    {
        unsigned int dpi = this->get_dpi();

        return this->style.strokeweight(unit2pixels(w, this->unit, dpi));
    }

    void Graphics::strokecap(int cap)
    {
        this->style.strokecap(cap);
    }

    void Graphics::strokejoin(int join)
    {
        this->style.strokejoin(join);
    }

    // Typography

    void Graphics::textsize(float size)
    {
        this->style.textsize(size);
    }

    void Graphics::textfont(Font *font)
    {
        this->textfont_impl(font);
    }

    void Graphics::text(std::string const &text, float x, float y)
    {
        this->text_impl(text, unit2pixels(x, this->unit, this->get_dpi()), unit2pixels(y, this->unit, this->get_dpi()));
    }
} // namespace Pyro
