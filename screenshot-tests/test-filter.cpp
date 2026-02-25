#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include "pyro/image.h"
#include "pyro/constants.h"

TEST_CASE("Images can be filtered")
{
    Pyro::Image *ducks = Pyro::Image::load("./tests/ducks.jpg");

    SECTION("Gray")
    {
        std::filesystem::path filename = "image_filter_gray.png";

        ducks->filter(Pyro::GRAY);
        ducks->save(current_folder / filename);

        delete ducks;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("Threshold")
    {
        std::filesystem::path filename = "image_filter_threshold.png";

        ducks->filter(Pyro::THRESHOLD);
        ducks->save(current_folder / filename);

        delete ducks;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("Invert")
    {
        std::filesystem::path filename = "image_filter_invert.png";

        ducks->filter(Pyro::INVERT);
        ducks->save(current_folder / filename);

        delete ducks;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
}
