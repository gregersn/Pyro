%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/transformer.h"
%End

    class Transformer2D
    {

    public:
        Transformer2D();
        void translate(float x, float y);
        void rotate(float a);
        void scale(float sx, float sy);

        void pushmatrix();
        void popmatrix();

        Pyro::Vector apply(Pyro::Vector v);

        Pyro::Vector screen(Pyro::Vector v);

        float screen_x(Pyro::Vector v);
        float screen_x(float x, float y, float z = 0.0f);
        float screen_y(Pyro::Vector v);
        float screen_y(float x, float y, float z = 0.0f);
    };
};
