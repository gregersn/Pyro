#ifndef PYRO_H
#define PYRO_H

#include "constants.h"
#include "string.h"
#include "math.h"
#include "color.h"
#include "image.h"
#include "shape.h"
#include "vector.h"
#include "graphics.h"
#include "noise.h"
#include "runner.h"
#include "utils.h"

namespace Pyro
{
    extern Graphics *pg; // Graphics context
    extern unsigned int width;
    extern unsigned int height;
    extern uint32_t *pixels;

    float real_width();
    float real_height();

    // Initialize the library with this.
    void size(unsigned int width, unsigned int height);
    void size(float width, float height, Unit unit, unsigned int dpi = 72);

    inline void save(const char *file) { pg->save(file); };
    inline void save(const char *file, unsigned int dpi) { pg->save(file, dpi); };

    void loadpixels();
    void updatepixels();
    inline void image(Image *img, float x, float y) { pg->image(img, x, y); };

    // Drawing functions

    // **** COLOR ****
    inline void background(Color const &c) { pg->background(c); };

    inline void background(int c, int a = 255) { pg->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0); };
    inline void background(int r, int g, int b, int a = 255) { pg->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); };

    inline void background(float c, float a = 1.0) { pg->background(c, c, c, a); };
    inline void background(float r, float g, float b, float a = 1.0) { pg->background(r, g, b, a); };

    // TODO: clear
    // TODO: colormode

    Color fill();
    Color fill(Color const &c);
    Color fill(float c, float a = 1.0);
    Color fill(float r, float g, float b, float a = 1.0);
    Color fill(int c, int a = 255);
    Color fill(int r, int g, int b, int a = 255);

    void nofill();
    void nostroke();

    Color stroke();
    Color stroke(Color const &c);
    Color stroke(float c, float a = 1.0);
    Color stroke(float r, float g, float b, float a = 1.0);
    Color stroke(int c, int a = 255);
    Color stroke(int r, int g, int b, int a = 255);
    // **** /COLOR ****

    // **** RENDERING ****
    inline void blendmode(int mode) { pg->blendmode(mode); };
    // TODO: clip

    // **** /RENDERING ****

    inline void smooth() { pg->smooth(); };
    inline void nosmooth() { pg->nosmooth(); };

    inline float strokeweight() { return pg->strokeweight(); };
    inline float strokeweight(float w) { return pg->strokeweight(w); };

    inline void beginshape(int kind = DEFAULT) { pg->beginshape(kind); };
    inline void endshape() { pg->endshape(); };
    inline void endshape(int close) { pg->endshape(close); };
    inline void vertex(float x, float y) { pg->vertex(x, y); };
    inline void curvevertex(float x, float y) { pg->curvevertex(x, y); };
    inline void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4) { pg->beziervertex(x2, y2, x3, y3, x4, y4); };

    inline void point(float x, float y) { pg->point(x, y); };
    inline void line(float x0, float y0, float x1, float y1) { pg->line(x0, y0, x1, y1); };
    inline void triangle(float x0, float y0, float x1, float y1, float x2, float y2) { pg->triangle(x0, y0, x1, y1, x2, y2); };
    inline void rect(float a, float b, float c, float d) { pg->rect(a, b, c, d); };
    inline void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) { pg->quad(x0, y0, x1, y1, x2, y2, x3, y3); };

    inline void arc(float a, float b, float c, float d, float start, float end, int mode = OPEN) { pg->arc(a, b, c, d, start, end, mode); };

    inline void ellipse(float x, float y, float w, float h, unsigned int segments = 32) { pg->ellipse(x, y, w, h, segments); };
    inline void circle(float x, float y, float r, unsigned int segments = 32) { ellipse(x, y, r, r, segments); };

    inline void translate(float x, float y) { pg->translate(x, y); };
    inline void scale(float sx, float sy) { pg->scale(sx, sy); };
    inline void rotate(float a) { pg->rotate(a); };
    inline void pushmatrix() { pg->pushmatrix(); };
    inline void popmatrix() { pg->popmatrix(); };

    inline float screen_x(float x, float y, float z = 0.0f) { return pg->screen_x(x, y, z); }
    inline float screen_y(float x, float y, float z = 0.0f) { return pg->screen_y(x, y, z); }
} // namespace Pyro

#endif // PYRO_H
