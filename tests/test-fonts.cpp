#include <catch2/catch.hpp>

#include "pyro/pyrofont.h"

TEST_CASE("Load a font") {
    SECTION("Read single font") {
        Pyro::Font * fnt = Pyro::Font::load("../fonts/Orkney Bold.otf");
        REQUIRE(fnt != nullptr);   
    }

    SECTION("Create font at size") {
        Pyro::Font *fnt = Pyro::create_font("../fonts/Orkney Bold.otf", 32);
        REQUIRE(fnt != nullptr);
    }
}