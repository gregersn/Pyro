#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Test varied DPI")
{
    SECTION("Check result matching")
    {
        Pyro::Graphics *pa = Pyro::creategraphics(508, 254, testmode);
        pa->set_dpi(100);
        pa->set_unit(Pyro::Unit::MM);
        pa->init();

        REQUIRE(pa->width() == 2000);
        REQUIRE(pa->height() == 1000);

        Pyro::Graphics *pb = Pyro::creategraphics(508, 254, testmode);
        pb->set_dpi(200);
        pb->set_unit(Pyro::Unit::MM);
        pb->init();

        REQUIRE(pb->width() == 4000);
        REQUIRE(pb->height() == 2000);

        pa->nosmooth();
        pb->nosmooth();
        pa->strokeweight(50, Pyro::Unit::MM);
        pb->strokeweight(50, Pyro::Unit::MM);

        pa->line(0, 0, pa->width(), pa->height());
        pb->line(0, 0, pb->width(), pb->height());

        Pyro::Image *pc = pb->resize(pa->width(), pa->height());

        std::string filename_a = "/tmp/508mm_by_254mm_100_dpi.png";
        std::string filename_b = "/tmp/508mm_by_254mm_200_dpi.png";

        pa->save(filename_a);
        pc->save(filename_b);

        CHECK_THAT(filename_a, LooksLike(filename_b));
    }
}
