%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyroimage.h>
#include <Python.h>
#include <numpy/ndarrayobject.h>
%End
    class Image {
        public:
            unsigned int width();
            unsigned int height();
            static Image *load(const char *);
            void save(const char *filename);
            void save(const char *filename, unsigned int dpi);

            unsigned int get(unsigned int x, unsigned int y);
            Image *get(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
            unsigned int operator[] (unsigned int);
            SIP_PYOBJECT load_pixels();
            %MethodCode
                import_array();
                npy_intp dims[] = {sipCpp->height(), sipCpp->width(), 4};
                unsigned char *data = (unsigned char*)sipCpp->load_bytes();
                
                sipRes = PyArray_SimpleNewFromData(3,
                                                   dims,
                                                   NPY_UINT8,
                                                   data);
            %End

            SIP_PYOBJECT load_bytes();
            %MethodCode
                import_array();
                npy_intp dims[] = {sipCpp->height(), sipCpp->width(), sipCpp->channels()};
                unsigned char *data = (unsigned char*)sipCpp->get_data();
                
                sipRes = PyArray_SimpleNewFromData(3,
                                                   dims,
                                                   NPY_UINT8,
                                                   data);
            %End

            Image *resize(unsigned int sx, unsigned int sy);

    };
    Image *createimage(unsigned int width, unsigned int height, int mode);

};