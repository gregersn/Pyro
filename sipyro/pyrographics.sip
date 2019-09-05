%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyrographics.h>
#include <Python.h>
%End
    class Graphics {
        public:
            Graphics(unsigned int width, unsigned int height, unsigned int channels);
            static Graphics *create(unsigned int width, unsigned int height);
            
            unsigned int width();
            unsigned int height();
            unsigned int channels;

            static Image *load(const char *);

            void save(const char *filename);
            void save(const char *filename, unsigned int dpi);


            void background(float r, float g, float b);
            void background(float r, float g, float b, float a);

            unsigned int get(unsigned int x, unsigned int y);

            void nostroke();
            void nofill();

            void fill(float c);
            void fill(float c, float a);
            void fill(float r, float g, float b);
            void fill(float r, float g, float b, float a);

            void line(float x0, float y0, float x1, float y1);
            void rect(float a, float b, float c, float d);

    };

};
