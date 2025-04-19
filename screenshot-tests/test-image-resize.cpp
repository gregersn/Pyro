#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include "pyro/image.h"

TEST_CASE("Images can be resized")
{
    SECTION("Resize a loaded image with different methods")
    {
        Pyro::Image *img = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *img2{nullptr};
        std::filesystem::path filename = "";
        REQUIRE(img != nullptr);

        SECTION("Nearest neighbor scale up")
        {
            img2 = img->resize(600, 600, Pyro::NEAREST);
            filename = "image_resize_scale_up_nn_loaded.png";

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);
        }

        SECTION("Nearest neighbor scale down")
        {
            img2 = img->resize(400, 400, Pyro::NEAREST);
            filename = "image_resize_scale_down_nn_loaded.png";

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);
        }

        SECTION("Bilinear scale up")
        {
            img2 = img->resize(600, 600, Pyro::BILINEAR);
            filename = "image_resize_scale_up_bi_loaded.png";

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);
        }

        SECTION("Bilinear scale down")
        {
            img2 = img->resize(400, 400, Pyro::BILINEAR);
            filename = "image_resize_scale_down_bi_loaded.png";

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);
        }

        img2->save(current_folder / filename);

        delete img2;
        delete img;

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
}
