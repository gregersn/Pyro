#include <catch2/catch.hpp>

#include <pyro.h>

TEST_CASE("The library can be initialized", "[setup]") {
    SECTION("using size()") {
        REQUIRE_NOTHROW(Pyro::size(100, 100));
    }
}

TEST_CASE("The pixel size of the canvas is calculated correctly", "[pyro]") {
    SECTION("using pixels sets absolute size") {
        unsigned int width = 500;
        unsigned int height = 250;
        Pyro::size(width, height);
        REQUIRE(width == Pyro::width);
        REQUIRE(height == Pyro::height);
    }

    SECTION("using inches calculates the right size") {
        unsigned int inwidth = 10;
        unsigned int inheight = 10;
        unsigned int dpi = 1;
        unsigned int pwidth = 10;
        unsigned int pheight = 10;
        Pyro::size(inwidth, inheight, Pyro::Unit::in, dpi);

        REQUIRE(pwidth == Pyro::width);
        REQUIRE(pheight == Pyro::height);
    }

    SECTION("using centimeters calculates the right size") {
        unsigned int cmwidth = 254;
        unsigned int cmheight = 254;
        unsigned int dpi = 2;
        unsigned int pwidth = dpi * 100;
        unsigned int pheight = dpi * 100;
        Pyro::size(cmwidth, cmheight, Pyro::Unit::cm, dpi);

        REQUIRE(pwidth == Pyro::width);
        REQUIRE(pheight == Pyro::height);
    }

    SECTION("using millimeters calculates the right size") {
        unsigned int cmwidth = 2540;
        unsigned int cmheight = 2540;
        unsigned int dpi = 2;
        unsigned int pwidth = dpi * 100;
        unsigned int pheight = dpi * 100;
        Pyro::size(cmwidth, cmheight, Pyro::Unit::mm, dpi);

        REQUIRE(pwidth == Pyro::width);
        REQUIRE(pheight == Pyro::height);
    }

}