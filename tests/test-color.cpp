#include <catch2/catch.hpp>

#include <pyrocolor.h>


TEST_CASE("Color objects", "[color]") {
    SECTION("Can be initialized with hex-strings") {
        Pyro::Color a = Pyro::Color("#00ffff");
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 1.0f);
        REQUIRE(a.a == 1.0f);
    }

    SECTION("Will initialize as opaque with one parameter") {
        Pyro::Color a = Pyro::Color(0.0f);
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 0.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);
    }

    SECTION("Will intialize as transparent greyscale with two parameters") {
        Pyro::Color a = Pyro::Color(0.4f, 0.5f);
        REQUIRE(a.r == 0.4f);
        REQUIRE(a.g == 0.4f);
        REQUIRE(a.b == 0.4f);
        REQUIRE(a.a == 0.5f);
    }

    SECTION("Will initialize as solid colors with three parameters") {
        Pyro::Color a = Pyro::Color(0.1f, 0.2f, 0.3f);
        REQUIRE(a.r == 0.1f);
        REQUIRE(a.g == 0.2f);
        REQUIRE(a.b == 0.3f);
        REQUIRE(a.a == 1.0f);
    }

    SECTION("Will initialize with alpha with four parameters") {
        Pyro::Color a = Pyro::Color(0.1f, 0.2f, 0.3f, 0.5f);
        REQUIRE(a.r == 0.1f);
        REQUIRE(a.g == 0.2f);
        REQUIRE(a.b == 0.3f);
        REQUIRE(a.a == 0.5f);
    }
}