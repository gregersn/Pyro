%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyrocolor.h>
%End
    class Color {
        public:
            float r;
            float g;
            float b;
            float a;
            Color(float v);
            Color(float v, float a);
            Color(float r, float g, float b);
            Color(float r, float g, float b, float a);
            void hsba(float h, float s, float b, float a);
            static unsigned int fto32(float r, float g, float b, float a);
    };

};

