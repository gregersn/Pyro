#include <catch2/catch_all.hpp>
#include "pyro/graphics.h"
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Place image in various ways")
{
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO);
    std::filesystem::path filename = "";
    Pyro::Image *img{nullptr};
    Pyro::Graphics *p{nullptr};

    SECTION("fullscreen")
    {
        filename = "image_place_default.png";

        img = Pyro::Image::load(actual_folder / "test_image.png");

        if (img == nullptr)
        {
            FAIL_CHECK("Test image does not exist");
            return;
        }

        p = Pyro::creategraphics(img->width(), img->height(), mode);
        p->begindraw();
        p->image(img, 0, 0);
        p->enddraw();
        p->save(current_folder / filename);
    }

    SECTION("Place image 50% at center rotated 45 degrees")
    {
        filename = "image_place_scaled_rotated.png";

        img = Pyro::Image::load(actual_folder / "test_image.png");
        if (img == nullptr)
        {
            FAIL_CHECK("Test image does not exist");
            return;
        }

        p = Pyro::creategraphics(img->width(), img->height(), mode);
        p->imagemode(Pyro::CENTER);
        p->translate(p->width() / 2.0f, p->height() / 2.0f);
        p->rotate(Pyro::QUARTER_PI);
        p->scale(0.5, 0.5);
        p->image(img, 0, 0);
    }

    SECTION("Place images with different channel count")
    {
        SECTION("4 channels")
        {
            filename = "image_place_4ch.png";

            img = Pyro::Image::load(actual_folder / "test_image.png");
            REQUIRE(img);
            REQUIRE(img->channels() == 4);

            p = Pyro::creategraphics(img->width(), img->height(), mode);
            p->imagemode(Pyro::CORNER);
            p->image(img, 0, 0);
        }

        SECTION("3 channels")
        {
            filename = "image_place_3ch.png";
            img = Pyro::Image::load(actual_folder / "test_image.png")->convert(Pyro::RGB);
            REQUIRE(img);
            REQUIRE(img->channels() == 3);

            p = Pyro::creategraphics(img->width(), img->height(), mode);
            p->imagemode(Pyro::CORNER);
            p->image(img, 0, 0);
        }

        SECTION("1 channel")
        {
            filename = "image_place_1ch.png";
            img = Pyro::Image::load(actual_folder / "test_image.png")->convert(Pyro::GRAY);
            REQUIRE(img);
            REQUIRE(img->channels() == 1);

            p = Pyro::creategraphics(img->width(), img->height(), mode);
            p->imagemode(Pyro::CORNER);
            p->image(img, 0, 0);
        }
    }
    p->save(current_folder / filename);
    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));

    delete p;
    delete img;
}
