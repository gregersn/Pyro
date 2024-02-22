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

    /**
     * Create a graphics object
     *
     * @param width The width of the canvas
     * @param height The height of the canvas
     */
    Graphics *create(unsigned int width, unsigned int height);
    Graphics *create(unsigned int width, unsigned int height, GraphicsMode mode);

    Graphics *create(unsigned int width, unsigned int height, Unit unit = Unit::px, unsigned int dpi = 72);
    // Initialize the library with this.
    void size(unsigned int width, unsigned int height, Unit unit = Unit::px, unsigned int dpi = 72);

    void save(const char *file);
    void save(const char *file, unsigned int dpi);

    void loadpixels();
    void updatepixels();
    void image(Image *img, float x, float y);

    // Drawing functions

    // **** COLOR ****
    void background(const Color &c);

    void background(int c, int a = 255);
    void background(int r, int g, int b, int a = 255);

    void background(float c, float a = 1.0);
    void background(float r, float g, float b, float a = 1.0);

    // TODO: clear
    // TODO: colormode

    void fill(const Color &c);
    void fill(float c, float a = 1.0);
    void fill(float r, float g, float b, float a = 1.0);
    void fill(int c, int a = 255);
    void fill(int r, int g, int b, int a = 255);

    void nofill();
    void nostroke();

    void stroke(const Color &c);
    void stroke(float c, float a = 1.0);
    void stroke(float r, float g, float b, float a = 1.0);
    void stroke(int c, int a = 255);
    void stroke(int r, int g, int b, int a = 255);
    // **** /COLOR ****

    // **** RENDERING ****
    void blendmode(BlendMode mode);
    // TODO: clip

    // **** /RENDERING ****

    void smooth();
    void nosmooth();

    void strokeweight(float w, Unit unit = Unit::current);

    void beginshape();
    void endshape();
    void endshape(int close);
    void vertex(float x, float y);
    void curvevertex(float x, float y);
    void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4);

    void point(float x, float y);
    void line(float x0, float y0, float x1, float y1);
    void triangle(float x0, float y0, float x1, float y1, float x2, float y2);
    void rect(float a, float b, float c, float d);
    void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

    void arc(float a, float b, float c, float d, float start, float end, int mode = OPEN);

    void ellipse(float x, float y, float w, float h, unsigned int segments = 32, Unit unit = Unit::current);
    void circle(float x, float y, float r, unsigned int segments = 32, Unit unit = Unit::current);

    void translate(float x, float y, Unit unit = Unit::current);
    void scale(float sx, float sy);
    void rotate(float a);
    void pushmatrix();
    void popmatrix();

    float screen_x(float x, float y, float z = 0.0f);
    float screen_y(float x, float y, float z = 0.0f);
} // namespace Pyro

#endif // PYRO_H
