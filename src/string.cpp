#include "pyro/string.h"

#include <algorithm>

namespace Pyro
{
    std::string nf(uint64_t number, uint64_t digits)
    {
        std::string number_string = std::to_string(number);
        return std::string(std::max((int)(digits - number_string.size()), 0), '0') + number_string;
    }
} // namespace Pyro
