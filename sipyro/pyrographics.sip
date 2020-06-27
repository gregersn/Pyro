%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyrographics.h>
#include <Python.h>
%End
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
            void translate(float x, float);
            void rotate(float a);
            void scale(float sx, float sy);
            void pushmatrix();
            void popmatrix();

            void background(float c);
            void background(float c, float a);
            void background(float r, float g, float b);
            void background(float r, float g, float b, float a);

            unsigned int get(unsigned int x, unsigned int y);
            void set(unsigned int x, unsigned int y, unsigned int c);

            void smooth();
            void nosmooth();

            void nostroke();
            void nofill();
            void strokeweight(float w); 
            void colormode(int mode);

            void stroke(float c);
            void stroke(float c, float a);
            void stroke(float r, float g, float b);
            void stroke(float r, float g, float b, float a);

            void fill(float c);
            void fill(float c, float a);
            void fill(float r, float g, float b);
            void fill(float r, float g, float b, float a);

            void line(float x0, float y0, float x1, float y1);
            void curve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
            void bezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void triangle(Vector a, Vector b, Vector c);
            void triangle(float x0, float y0, float x1, float y1, float x2, float y2);

            void quad(Vector a, Vector b, Vector c, Vector d);
            void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void rectmode(int mode);
            void rect(float a, float b, float c, float d);

            void ellipsemode(int mode);
            void ellipse(float x, float y, float w, float h, unsigned int segments);

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
