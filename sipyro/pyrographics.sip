%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyrographics.h>
#include <Python.h>
%End
    enum class RectMode {
        CORNER,
        CENTER
    };

    class Graphics {
        public:
            Graphics(unsigned int width, unsigned int height, unsigned int channels);
            static Graphics *create(unsigned int width, unsigned int height);
            
            unsigned int width();
            unsigned int height();
            unsigned int channels;

            static Image *load(const char *);
            Image *loadimage(const char *);
            void image(Image *img, float x, float y);

            void save(const char *filename);
            void save(const char *filename, unsigned int dpi);

            // Transformation
            void translate(float x, float);
            void rotate(float a);
            void pushmatrix();
            void popmatrix();

            void background(float c);
            void background(float c, float a);
            void background(float r, float g, float b);
            void background(float r, float g, float b, float a);

            unsigned int get(unsigned int x, unsigned int y);
            void set(unsigned int x, unsigned int y, unsigned int c);

            void nostroke();
            void nofill();
            void colormode(int mode);


            void fill(float c);
            void fill(float c, float a);
            void fill(float r, float g, float b);
            void fill(float r, float g, float b, float a);

            void line(float x0, float y0, float x1, float y1);

            void triangle(Vector a, Vector b, Vector c);
            void triangle(float x0, float y0, float x1, float y1, float x2, float y2);

            void quad(Vector a, Vector b, Vector c, Vector d);
            void quad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

            void rectmode(RectMode mode);
            void rect(float a, float b, float c, float d);

            void ellipse(float x, float y, float w, float h, unsigned int segments);


    };

};
