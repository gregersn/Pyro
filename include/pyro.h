#ifndef PYRO_H
#define PYRO_H

#include "pyroconstants.h"
#include "pyrocolor.h"
#include "pyroimage.h"
#include "pyroshape.h"
#include "pyrovector.h"
#include "pyrographics.h"

namespace Pyro {
    enum class Unit {
        px, cm, mm, in
    };

    extern Graphics *pg;
    extern unsigned int width;
    extern unsigned int height;
    extern uint32_t *pixels;

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi);
    void size(unsigned int width, unsigned int height);

    uint32_t color(unsigned int r, unsigned int g, unsigned int b);
    uint32_t color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

    inline void save(const std::string &file) { pg->save(file); };

    void loadpixels();
    void updatepixels();
    inline void image(Image *img, float x, float y) { pg->image(img, x, y); };

    inline float random() { return pg->random(); }; 
    
    template<typename T>
    inline T random(T a, T b) { return pg->random(a, b); };
    
    inline void randomseed(unsigned int seed) { pg->randomseed(seed); };

    template<typename T>
    inline T max(T a, T b) { return a ? a > b : b; }

    template<typename T>
    inline T map(T value, T start1, T stop1, T start2, T stop2) { return pg->map(value, start1, stop1, start2, stop2); };

    inline float radians(float degrees) { return pg->radians(degrees); };
    
    inline void nostroke() { pg->nostroke(); };
    inline void nofill() { pg->nofill(); };

    // Drawing functions
    inline void background(int c) { pg->background(c / 255.0f, c / 255.0f, c / 255.0f, 1.0f); };
    inline void background(int c, int a) { pg->background(c / 255.0f, c / 255.0f, c / 255.0f, a / 255.0); };
    inline void background(int r, int g, int b) { pg->background(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); };
    inline void background(int r, int g, int b, int a) { pg->background(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); };

    inline void background(float c) { pg->background(c, c, c, 1.0); };
    inline void background(float c, float a) { pg->background(c, c, c, a); };
    inline void background(float r, float g, float b) { pg->background(r, g, b, 1.0); };
    inline void background(float r, float g, float b, float a) { pg->background(r, g, b, a); };

    inline void smooth() { pg->smooth(); };
    inline void nosmooth() { pg->nosmooth(); };

    inline void fill(float c) {  pg->fill(c, c, c, 1.0); };
    inline void fill(float c, float a) { pg->fill(c, c, c, a); };
    inline void fill(float r, float g, float b) { pg->fill(r, g, b, 1.0); };
    inline void fill(float r, float g, float b, float a) {pg->fill(r, g, b, a); };

    inline void fill(int c) { pg->fill(c); };
    inline void fill(int c, int a) { pg->fill(c, a); };
    inline void fill(int r, int g, int b) { pg->fill(r, g, b); };
    inline void fill(int r, int g, int b, int a) { pg->fill(r, g, b, a); };

    inline void stroke(float c) {  pg->stroke(c, c, c, 1.0); };
    inline void stroke(float c, float a) { pg->stroke(c, c, c, a); };
    inline void stroke(float r, float g, float b) { pg->stroke(r, g, b, 1.0); };
    inline void stroke(float r, float g, float b, float a) {pg->stroke(r, g, b, a); };

    inline void stroke(int c) { pg->stroke(c); };
    inline void stroke(int c, int a) { pg->stroke(c, a); };
    inline void stroke(int r, int g, int b) { pg->stroke(r, g, b); };
    inline void stroke(int r, int g, int b, int a) { pg->stroke(r, g, b, a); };

    inline void strokeweight(float w) { pg->strokeweight(w); };

    inline void beginshape() { pg->beginshape(); };
    inline void endshape(int close) { pg->endshape(close); };
    inline void vertex(float x, float y) { pg->vertex(x, y); };

    inline void point(float x, float y) { pg->point(x, y); };
    inline void line(float x0, float y0, float x1, float y1) { pg->line(x0, y0, x1, y1); };
    inline void triangle(float x0, float y0, float x1, float y1, float x2, float y2) { pg->triangle(x0, y0, x1, y1, x2, y2); };
    inline void rect(float a, float b, float c, float d) { pg->rect(a, b, c, d); };
    inline void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) { pg->quad(x0, y0, x1, y1, x2, y2, x3, y3); };

    inline void arc(float a, float b, float c, float d, float start, float end, int mode) { pg->arc(a, b, c, d, start, end); };
    inline void arc(float a, float b, float c, float d, float start, float end) { arc(a, b, c, d, start, end, OPEN); };

    inline void ellipse(float x, float y, float w, float h) { pg->ellipse(x, y, w, h); };
    inline void ellipse(float x, float y, float r) { ellipse(x, y, r, r); };

    inline void translate(float x, float y) { pg->translate(x, y); };
    inline void rotate(float a) { pg->rotate(a); };
    inline void pushmatrix() { pg->pushmatrix(); };
    inline void popmatrix() { pg->popmatrix(); };


}

#endif // PYRO_H