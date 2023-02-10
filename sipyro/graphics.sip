%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/graphics.h"
#include <Python.h>
%End
    enum class BlendMode
    {
        ADD,
        BLEND,
        DARKEST,
        DIFFERENCE,
        EXCLUSION,
        LIGHTEST,
        MULTIPLY,
        REPLACE,
        SCREEN,
        SUBTRACT
    };

    class Graphics : public Pyro::Image {
        public:
            Graphics(unsigned int width, unsigned int height, unsigned int channels, unsigned int dpi);
            static Graphics *create(unsigned int width, unsigned int height);
            
            unsigned int width();
            unsigned int height();
            unsigned int channels();

            static Image *load(const char *);
            Image *loadimage(const char *);
            void image(Image *img, float x, float y);

            void save(const char *filename);
            void save(const char *filename, unsigned int dpi);

            // Transformation
            void translate(float x, float, Unit unit = Pyro::Unit::current);
            void rotate(float a);
            void scale(float sx, float sy);
            void pushmatrix();
            void popmatrix();

            void background(float c, float a = 1.0);
            void background(float r, float g, float b, float a = 1.0);

            unsigned int get(unsigned int x, unsigned int y);
            void set(unsigned int x, unsigned int y, unsigned int c);

            void smooth();
            void nosmooth();

            void nostroke();
            void nofill();
            void strokeweight(float w, Unit unit = Pyro::Unit::current); 
            void strokecap(int cap);
            void strokejoin(int join);
            void colormode(int mode);
            void blendmode(BlendMode mode);


            void stroke(Color c /Constrained/);

            void stroke(float c  /Constrained/, float a  /Constrained/ = 1.0);
            void stroke(float r /Constrained/, float g /Constrained/, float b /Constrained/, float a  /Constrained/ = 1.0);

            void stroke(int c, int a = 255);
            void stroke(int r, int g, int b, int a = 255);


            void fill(Color c /Constrained/);

            void fill(float c  /Constrained/, float a  /Constrained/ = 1.0);
            void fill(float r /Constrained/, float g /Constrained/, float b /Constrained/, float a  /Constrained/ = 1.0);

            void fill(int c, int a = 255);
            void fill(int r, int g, int b, int a = 255);

            void line(float x0, float y0, float x1, float y1, Unit unit = Pyro::Unit::current);
            void curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
            void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void triangle(Vector a, Vector b, Vector c);
            void triangle(float x0, float y0, float x1, float y1, float x2, float y2);

            void quad(Vector a, Vector b, Vector c, Vector d);
            void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void arc(float a, float b, float c, float d, float start, float end, int mode = Pyro::OPEN, Pyro::Unit unit = Pyro::Unit::current);
            
            void rectmode(int mode);
            void rect(float a, float b, float c, float d, Pyro::Unit unit = Pyro::Unit::current);

            void ellipsemode(int mode);
            void ellipse(float x, float y, float w, float h, unsigned int segments, Unit unit = Pyro::Unit::current);

            void beginshape();
            void vertex(Vector v);
            void vertex(float x, float y);
            void curvevertex(float x, float y);
            void beziervertex(float x2, float y2, float x3, float y3, float x4, float y4);
            void endshape(int close);
            void endshape();

            void textsize(float size);
            void text(std::string text, float x, float y);           
    };

};

%MappedType std::string {
%TypeHeaderCode
#include <string>
#include <Python.h>

%End

%ConvertFromTypeCode
    // convert an std::string to a Python (unicode) string
    PyObject* newstring;
    newstring = PyUnicode_DecodeUTF8(sipCpp->c_str(), sipCpp->length(), NULL);
    if(newstring == NULL) {
        PyErr_Clear();
        newstring = PyUnicode_FromString(sipCpp->c_str());
    }
    return newstring;
%End

%ConvertToTypeCode
    // Allow a Python string (or a unicode string) whenever a string is
    // expected.
    // If argument is a Unicode string, just decode it to UTF-8
    // If argument is a Python string, assume it's UTF-8
    if (sipIsErr == NULL)
        return (PyUnicode_Check(sipPy) || PyUnicode_Check(sipPy));
    if (sipPy == Py_None) {
        *sipCppPtr = new std::string;
        return 1;
    }
    if (PyUnicode_Check(sipPy)) {
        PyObject* s = PyUnicode_AsEncodedString(sipPy, "UTF-8", "");
        *sipCppPtr = new std::string(PyBytes_AsString(s));
        Py_DECREF(s);
        return 1;
    }
    if (PyUnicode_Check(sipPy)) {
        *sipCppPtr = new std::string(PyBytes_AsString(sipPy));
        return 1;
    }
    return 0;
%End
};
