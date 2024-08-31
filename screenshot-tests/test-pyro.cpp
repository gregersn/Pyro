#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Using differnt resolutions can give the same picture")
{
    SECTION("Using inches and pixels")
    {
        unsigned int inwidth = 15;
        unsigned int inheight = 10;
        unsigned int dpi = 10;

        unsigned int pwidth = inwidth * dpi;
        unsigned int pheight = inheight * dpi;

        Pyro::size(inwidth, inheight, Pyro::Unit::IN, dpi);
        Pyro::strokeweight(1.0 / dpi);
        Pyro::rect(0, 0, 1, 1);
        Pyro::translate(2, 2);
        Pyro::rect(0, 0, 1, 1);
        Pyro::translate(Pyro::real_width() / 2.0, Pyro::real_height() / 2.0);
        Pyro::ellipse(0, 0, 2, 2);
        Pyro::save("/tmp/pyro-test-pyro-inches.png");

        Pyro::size(pwidth, pheight, Pyro::Unit::PX, dpi);
        Pyro::strokeweight(1.0);
        Pyro::rect(0, 0, 10, 10);
        Pyro::translate(20, 20);
        Pyro::rect(0, 0, 10, 10);
        Pyro::translate(Pyro::real_width() / 2.0, Pyro::real_height() / 2.0);
        Pyro::ellipse(0, 0, 2 * dpi, 2 * dpi);
        Pyro::save("/tmp/pyro-test-pyro-pixels.png");
        CHECK_THAT("/tmp/pyro-test-pyro-inches.png", LooksLike("/tmp/pyro-test-pyro-pixels.png"));
    }
}