#include <catch2/catch.hpp>

#include "pyro/pyrostring.h"

TEST_CASE("A number can be zero padded to certain string length")
{
    SECTION("Padding single digit")
    {
        std::string t = Pyro::nf(1, 3);
        REQUIRE(t.length() == 3);
        REQUIRE(t.compare("001") == 0);
    }

    SECTION("Padding multiple digits")
    {
        std::string t = Pyro::nf(12, 3);
        REQUIRE(t.length() == 3);
        REQUIRE(t.compare("012") == 0);
    }

    SECTION("Don't bad long number")
    {
        std::string t = Pyro::nf(123, 3);
        REQUIRE(t.length() == 3);
        REQUIRE(t.compare("123") == 0);
    }

    SECTION("Longer numbers as is")
    {
        std::string t = Pyro::nf(1234, 3);
        REQUIRE(t.length() == 4);
        REQUIRE(t.compare("1234") == 0);
    }
}
