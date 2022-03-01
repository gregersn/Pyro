%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/vector.h"
%End

    class Vector {
        public:
            float x;
            float y;
            float z;
            
            Vector();
            Vector(float x, float y, float z = 0.0f);
            SIP_PYOBJECT __repr__() const /TypeHint="str"/;
%MethodCode
                PyObject *x = PyFloat_FromDouble(sipCpp->x);
                PyObject *y = PyFloat_FromDouble(sipCpp->y);
                PyObject *z = PyFloat_FromDouble(sipCpp->z);
                
                sipRes = PyUnicode_FromFormat("pyro.Vector(%R, %R, %R)", x, y, z);

                Py_XDECREF(x);
                Py_XDECREF(y);
                Py_XDECREF(z);
%End
            
            float dist(const Vector &other) const;
            float mag() const;
            float dot(const Vector &other) const;
            Vector cross(const Vector &other) const;
            float heading() const;
            float angle(const Vector &other) const;
            Vector rotate(const float angle) const;
            
            Vector normalize() const;
            Vector copy() const;
            Vector div(float v) const;
            Vector setmag(float mag) const;
            Vector limit(float mag) const;
            static Vector fromangle(float a);
            static Vector random2d();
            static Vector random3d();
            Vector operator+ (const Vector &other) const;
            void operator+= (const Vector &other);
            Vector operator- (const Vector &other) const;
            Vector operator/ (const float v) const;
            Vector operator* (const float v) const;
            bool operator== (const Vector &rhs) const;
            bool operator!= (const Vector &rhs) const;
            bool operator< (const Vector &rhs) const;
            bool operator> (const Vector &rhs) const;
            bool operator<= (const Vector &rhs) const;
            bool operator>= (const Vector &rhs) const;
    };
};
