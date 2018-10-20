#include "catch.hpp"

#include <pyrocolor.h>


TEST_CASE("Color objects", "[color]") {
    SECTION("Can be initialized with hex-strings") {
        Pyro::Color a = Pyro::Color("#00ffff");
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 1.0f);
        REQUIRE(a.a == 1.0f);
    }
}