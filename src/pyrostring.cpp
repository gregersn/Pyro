#include "pyro/pyrostring.h"

#include <algorithm>

namespace Pyro
{
    std::string nf(uint number, uint digits) {
        std::string number_string = std::to_string(number);
        return std::string(std::max((int)(digits - number_string.size()), 0), '0') + number_string;
    }
} // namespace Pyro
