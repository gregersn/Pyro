%Module(name=pyro, call_super_init=True)
%DefaultSupertype sip.simplewrapper

%DefaultEncoding "UTF-8"
%Include pyroconstants.sip
%Include pyromath.sip
%Include pyrocolor.sip
%Include pyroshape.sip
%Include pyrovector.sip
%Include pyroimage.sip
%Include pyrographics.sip

%HideNamespace(name=Pyro)
%ModuleCode
#include <pyro/pyro.h>
%End

namespace Pyro {
    enum class Unit { px, cm, mm, in };

    unsigned int width;
    unsigned int height;

    Graphics *create(unsigned int width, unsigned int height);

    void size(unsigned int width, unsigned int height, Unit unit, unsigned int dpi);
    void size(unsigned int width, unsigned int height);

    unsigned int color(unsigned int r, unsigned int g, unsigned int b);
    unsigned int color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

    void save(const char *file);
    void save(const char *file, unsigned int dpi);

    void nostroke();
    void nofill();

    void background(Color c);

    void background(int c);
    void background(int c, int a);

    void smooth();
    void nosmooth();

    void fill(Color c);
    void stroke(Color c);

    void point(float x, float y);
    void line(float x0, float y0, float x1, float y1);
    void triangle(float x0, float y0, float x1, float y1, float x2, float y2);
    void rect(float a, float b, float c, float d);

    void ellipse(float x, float y, float w, float h, unsigned int segments = 32);

    void translate(float x, float y);
    void rotate(float a);
    void pushmatrix();
    void popmatrix();
};
