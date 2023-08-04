#include <catch2/catch_all.hpp>
#include "test-settings.h"

SCENARIO("Check that different units can be used.")
{
    SECTION("Check using current unit")
    {
        Pyro::Graphics *p = Pyro::Graphics::create(4, 4, testmode, 72u, Pyro::Unit::in);
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
