#include <catch2/catch_all.hpp>
#include "test-settings.h"

SCENARIO("Check that different units can be used.")
{
    SECTION("Check using current unit")
    {
        Pyro::Graphics *p = Pyro::creategraphics(4, 4, testmode);
        p->set_dpi(72);
        p->set_unit(Pyro::Unit::IN);
        p->init();
        std::string filename = "using_current_unit.png";
        p->nostroke();
        p->background(192);
        p->fill(0, 0, 0, 255);
        p->ellipse(2, 2, 2, 8);
        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
}
