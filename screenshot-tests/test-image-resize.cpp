#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include "pyro/image.h"

TEST_CASE("Images can be resized")
{
    SECTION("Resize a loaded image with different methods")
    {
        Pyro::Image *img = Pyro::Image::load("./tests/ducks.jpg");
        REQUIRE(img != nullptr);

        SECTION("Nearest neighbor scale up")
        {
            Pyro::Image *img2 = img->resize(600, 600, Pyro::NEAREST);
            std::filesystem::path filename = "image_resize_scale_up_nn_loaded.png";

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);

            img2->save(current_folder / filename);

            delete img2;
            delete img;

            CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
        }

        SECTION("Nearest neighbor scale down")
        {
            Pyro::Image *img2 = img->resize(400, 400, Pyro::NEAREST);
            std::filesystem::path filename = "image_resize_scale_down_nn_loaded.png";

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);

            img2->save(current_folder / filename);

            delete img2;
            delete img;

            CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
        }

        SECTION("Bilinear scale up")
        {
            Pyro::Image *img2 = img->resize(600, 600, Pyro::BILINEAR);
            std::filesystem::path filename = "image_resize_scale_up_bi_loaded.png";

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);

            img2->save(current_folder / filename);

            delete img2;
            delete img;

            CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
        }

        SECTION("Bilinear scale down")
        {
            Pyro::Image *img2 = img->resize(400, 400, Pyro::BILINEAR);
            std::filesystem::path filename = "image_resize_scale_down_bi_loaded.png";

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);

            img2->save(current_folder / filename);

            delete img2;
            delete img;

            CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
        }
    }
}
