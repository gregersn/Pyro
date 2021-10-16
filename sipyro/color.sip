%DefaultEncoding "UTF-8"

namespace Pyro {
%TypeHeaderCode
#include "pyro/color.h"
%End
    class Color {
        public:
            float r();
            float g();
            float b();
            float a();

            void lightness(float l);
            Color(const std::string /Constrained/);
            Color(float);
            Color(float v, float a);
            Color(float r, float g, float b);
            Color(float /Constrained/, float /Constrained/, float /Constrained/, float /Constrained/);
            Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
            void hsba(float h, float s, float b, float a);
            static unsigned int fto32(float r, float g, float b, float a);
            static Color from_uint(unsigned int c);
            unsigned int to_uint();
            Color lerp(Color other, float t);

    };

};

