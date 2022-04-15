%Module(name=pyro, call_super_init=True, keyword_arguments="All")

%DefaultEncoding "UTF-8"
%Include constants.sip
%Include math.sip
%Include color.sip
%Include shape.sip
%Include vector.sip
%Include transformer.sip
%Include image.sip
%Include graphics.sip
%Include sdl.sip

%HideNamespace(name=Pyro)
%ModuleCode
#include "pyro/pyro.h"
#include "pyro/shape.h"
#include "pyro/image.h"
%End

namespace Pyro {
    enum class Unit { px, cm, mm, in };

    unsigned int width;
    unsigned int height;
    Graphics *pg;

    Graphics *create(unsigned int width, unsigned int height, Unit unit = Pyro::Unit::px, unsigned int dpi = 72);
    void size(unsigned int width, unsigned int height, Unit unit = Pyro::Unit::px, unsigned int dpi = 72);

    void save(const char *file);
    void save(const char *file, unsigned int dpi);

    unsigned int color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);

    void nostroke();
    void nofill();

    void background(Color c /Constrained/);

    void background(int c, int a = 255);
    void background(int r, int g, int b, int a = 255);

    void smooth();
    void nosmooth();

    void fill(Color c /Constrained/);
    void fill(int c, int a = 255);
    void fill(int r, int g, int b, int a = 255);

    void blendmode(BlendMode mode);

    void stroke(Color c /Constrained/);
    void stroke(int c, int a = 255);
    void stroke(int r, int g, int b, int a = 255);

    void strokeweight(float w, Unit unit = Pyro::Unit::current);

    void point(float x, float y);
    void line(float x0, float y0, float x1, float y1);
    void triangle(float x0, float y0, float x1, float y1, float x2, float y2);
    void rect(float a, float b, float c, float d);
    float curvepoint(float p0, float p1, float p2, float p3, float t);

    void ellipse(float x, float y, float w, float h, unsigned int segments = 32);

    void translate(float x, float y, Unit unit = Pyro::Unit::current);
    void scale(float sx, float sy);
    void rotate(float a);
    void pushmatrix();
    void popmatrix();

    double noise(double x, double y);
    double noise(double x, double y, double z);
    double noise(double x, double y, double z, double w);
    void noiseseed(long seed);
    void noisedetail(unsigned int lod, float falloff);
    };
