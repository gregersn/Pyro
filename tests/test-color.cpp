#include <catch2/catch.hpp>

#include <pyro/pyrocolor.h>


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

    SECTION("Will initialize with four uint") {
        Pyro::Color a = Pyro::Color(255, 0u, 0u, 255);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 0.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);
    }

    SECTION("Will convert to uint32") {
        Pyro::Color a = Pyro::Color(1.0f, 0.0f, 1.0f, 1.0f);
        uint32_t c = a.to_uint();
        REQUIRE(c == 0xffff00ff);
    }


    SECTION("Can be converted to and from uint32") {
        Pyro::Color a = Pyro::Color::from_uint(0xff00ff00);
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);
    }

    SECTION("Colors can be lerped") {
        Pyro::Color a = Pyro::Color(1.0f, 0.0f, 0.0f, 1.0f);
        Pyro::Color b = Pyro::Color(0.0f, 1.0f, 0.0f, 0.0f);

        Pyro::Color c = a.lerp(b, 0.5f);

        REQUIRE(c.r == 0.5f);
        REQUIRE(c.g == 0.5f);
        REQUIRE(c.b == 0.0f);
        REQUIRE(c.a == 0.5f);
    }

    SECTION("Colors can be set with HSB") {
        Pyro::Color a = Pyro::Color(1.0f);
        a.hsba(0.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 0.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(0.125f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 0.25f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(0.25f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 0.5f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(0.5f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(0.75f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 0.5f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(1.0f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 0.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(1.5f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 1.0f);
        REQUIRE(a.b == 1.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(2.0f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 0.0f);
        REQUIRE(a.g == 0.0f);
        REQUIRE(a.b == 1.0f);
        REQUIRE(a.a == 1.0f);

        a.hsba(2.5f / 3.0f, 1.0f, 1.0f, 1.0f);
        REQUIRE(a.r == 1.0f);
        REQUIRE(a.g == 0.0f);
        REQUIRE(a.b == 1.0f);
        REQUIRE(a.a == 1.0f);
    }
}