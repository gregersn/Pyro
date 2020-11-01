%DefaultEncoding "UTF-8"


/*template<Type *>

%MappedType T<double>
{
    %ConvertFromTypeCode
        return sipCppV;
    %End

    %ConvertToTypeCode
        return sipPy;
    %End
};*/

namespace Pyro {
%TypeHeaderCode
#include <pyro/pyromath.h>
%End
    double random(double range);
    double random(double low, double high);
    void randomseed(unsigned int seed);

    float radians(float degrees);
    float degrees(float radian);

   double map(double value, double start1, double stop1, double start2, double stop2);
   double lerp(double start, double stop, float amt);
   
};
