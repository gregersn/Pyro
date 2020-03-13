#include <catch2/catch.hpp>
#include <pyro/pyrographics_cairo.h>

TEST_CASE("Load font", "[cairo]") {
    Pyro::Font *f = Pyro::FontCairo::load("../screenshot-tests/actual/Zantroke-gmR5.otf");
    REQUIRE(f != nullptr);
}

TEST_CASE("Load font through graphics", "[cairo]") {
    Pyro::GraphicsCairo *pgc = new Pyro::GraphicsCairo(100, 100, 4, 72);
    //Pyro::Font *f = pgc->loadfont_impl("../screenshot-tests/actual/Zantroke-gmR5.otf");
    //REQUIRE(f != nullptr);   
}
