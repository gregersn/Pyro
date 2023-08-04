#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Test varied DPI")
{
    SECTION("Check result matching")
    {
        Pyro::Graphics *pa = Pyro::Graphics::create(508, 254, testmode, 100, Pyro::Unit::mm);

        REQUIRE(pa->width() == 2000);
        REQUIRE(pa->height() == 1000);

        Pyro::Graphics *pb = Pyro::Graphics::create(508, 254, testmode, 200, Pyro::Unit::mm);

        REQUIRE(pb->width() == 4000);
        REQUIRE(pb->height() == 2000);

        pa->nosmooth();
        pb->nosmooth();
        pa->strokeweight(50, Pyro::Unit::mm);
        pb->strokeweight(50, Pyro::Unit::mm);

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
