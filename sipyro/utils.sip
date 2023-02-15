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
#include "pyro/utils.h"
%End
   double unit2pixels(double value, Pyro::Unit unit, unsigned int dpi = 72);
   double pixels2unit(double value, Pyro::Unit unit, unsigned int dpi = 72);
   
};
