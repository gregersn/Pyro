#include "pyro/pyro.h"
#include <cstdlib>

namespace Pyro
{
    Graphics *pg = nullptr;
    unsigned int width = 640;
    unsigned int height = 480;
    uint32_t *pixels = nullptr;

    void exit()
    {
        if (pg != nullptr)
        {
            delete pg;
        }
    }

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi)
    {
        float multiplier = 1.0f;
        switch (unit)
        {
        case Unit::mm:
            multiplier = dpi / 25.4;
            break;
        case Unit::cm:
            multiplier = dpi / 2.54;
            break;
        case Unit::in:
            multiplier = dpi;
            break;
        case Unit::px:
        default:
            break;
        }

        unsigned int w = (unsigned int)width * multiplier;
        unsigned int h = (unsigned int)height * multiplier;

        if (pg == nullptr)
        {
            std::atexit(exit);
        }

        if (pg != nullptr)
        {
            delete pg;
        }
        Pyro::width = w;
        Pyro::height = h;
        pg = Graphics::create(width, height, GraphicsMode::CAIRO, dpi, unit);
    }

    void loadpixels()
    {
        pixels = pg->load_pixels();
    }

    void updatepixels()
    {
        pixels = nullptr;
    }

    /**
     * Create a graphics object
     *
     * @param width The width of the canvas
     * @param height The height of the canvas
     */
    Graphics *create(unsigned int width, unsigned int height)
    {
        return Graphics::create(width, height);
    }

    Graphics *create(unsigned int width, unsigned int height, GraphicsMode mode)
    {
        return Graphics::create(width, height, mode);
    }

    Graphics *create(unsigned int width, unsigned int height, Unit unit, unsigned int dpi)
    {
        return Graphics::create(width, height, GraphicsMode::CAIRO, dpi, unit);
    }

    // Initialize the library with this.
    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi);

    void save(const char *file) { pg->save(file); };
    void save(const char *file, unsigned int dpi) { pg->save(file, dpi); };

    void loadpixels();
    void updatepixels();
    void image(Image *img, float x, float y) { pg->image(img, x, y); };

    // Drawing functions

    // **** COLOR ****
    void background(const Color &c) { pg->background(c); };

    void background(int c, int a) { pg->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0); };
    void background(int r, int g, int b, int a) { pg->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); };

    void background(float c, float a) { pg->background(c, c, c, a); };
    void background(float r, float g, float b, float a) { pg->background(r, g, b, a); };

    // TODO: clear
    // TODO: colormode

    void fill(const Color &c) { pg->fill(c); };
    void fill(float c, float a) { pg->fill(c, c, c, a); };
    void fill(float r, float g, float b, float a) { pg->fill(r, g, b, a); };
    void fill(int c, int a) { pg->fill(c, a); };
    void fill(int r, int g, int b, int a) { pg->fill(r, g, b, a); };

    void nofill() { pg->nofill(); };
    void nostroke() { pg->nostroke(); };

    void stroke(const Color &c) { pg->stroke(c); };
    void stroke(float c, float a) { pg->stroke(c, c, c, a); };
    void stroke(float r, float g, float b, float a) { pg->stroke(r, g, b, a); };
    void stroke(int c, int a) { pg->stroke(c, a); };
    void stroke(int r, int g, int b, int a) { pg->stroke(r, g, b, a); };
    // **** /COLOR ****

    // **** RENDERING ****
    void blendmode(BlendMode mode) { pg->blendmode(mode); };
    // TODO: clip

    // **** /RENDERING ****

    void smooth() { pg->smooth(); };
    void nosmooth() { pg->nosmooth(); };

    void strokeweight(float w, Unit unit) { pg->strokeweight(w, unit); };

    void beginshape() { pg->beginshape(); };
    void endshape() { pg->endshape(); };
    void endshape(int close) { pg->endshape(close); };
    void vertex(float x, float y) { pg->vertex(x, y); };
    void curvevertex(float x, float y) { pg->curvevertex(x, y); };
    void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) { pg->beziervertex(x2, y2, x3, y3, x4, y4); };

    void point(float x, float y) { pg->point(x, y); };
    void line(float x0, float y0, float x1, float y1) { pg->line(x0, y0, x1, y1); };
    void triangle(float x0, float y0, float x1, float y1, float x2, float y2) { pg->triangle(x0, y0, x1, y1, x2, y2); };
    void rect(float a, float b, float c, float d) { pg->rect(a, b, c, d); };
    void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) { pg->quad(x0, y0, x1, y1, x2, y2, x3, y3); };

    void arc(float a, float b, float c, float d, float start, float end, int mode) { pg->arc(a, b, c, d, start, end, mode); };

    void ellipse(float x, float y, float w, float h, unsigned int segments, Unit unit) { pg->ellipse(x, y, w, h, segments, unit); };
    void circle(float x, float y, float r, unsigned int segments, Unit unit) { ellipse(x, y, r, r, segments, unit); };

    void translate(float x, float y, Unit unit) { pg->translate(x, y, unit); };
    void scale(float sx, float sy) { pg->scale(sx, sy); };
    void rotate(float a) { pg->rotate(a); };
    void pushmatrix() { pg->pushmatrix(); };
    void popmatrix() { pg->popmatrix(); };

    float screen_x(float x, float y, float z) { return pg->screen_x(x, y, z); }
    float screen_y(float x, float y, float z) { return pg->screen_y(x, y, z); }

}
