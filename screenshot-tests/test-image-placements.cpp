#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Place image full screen")
{
    std::string filename = "image_place_default.png";

    Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");
    if (img == nullptr)
    {
        FAIL_CHECK("Test image does not exist");
        return;
    }

    Pyro::Graphics *p = Pyro::creategraphics(img->width(), img->height(), testmode);
    p->image(img, 0, 0);

    p->save(current_folder + filename);

    delete img;
    delete p;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}

TEST_CASE("Place image 50% at center rotated 45 degrees")
{
    std::string filename = "image_place_scaled_rotated.png";

    Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");
    if (img == nullptr)
    {
        FAIL_CHECK("Test image does not exist");
        return;
    }

    Pyro::Graphics *p = Pyro::creategraphics(img->width(), img->height(), testmode);
    p->imagemode(Pyro::CENTER);
    p->translate(p->width() / 2.0f, p->height() / 2.0f);
    p->rotate(Pyro::QUARTER_PI);
    p->scale(0.5, 0.5);
    p->image(img, 0, 0);

    p->save(current_folder + filename);

    delete img;
    delete p;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}

TEST_CASE("Place images with different channel count")
{
    SECTION("4 channels")
    {
        std::string filename = "image_place_4ch.png";

        Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");
        REQUIRE(img->channels() == 4);

        Pyro::Graphics *p = Pyro::creategraphics(img->width(), img->height(), testmode);
        p->imagemode(Pyro::CORNER);
        p->image(img, 0, 0);
        p->save(current_folder + filename);
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));

        delete p;
        delete img;
    }

    SECTION("3 channels")
    {
        std::string filename = "image_place_3ch.png";
        Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png")->convert(Pyro::RGB);
        REQUIRE(img->channels() == 3);

        Pyro::Graphics *p = Pyro::creategraphics(img->width(), img->height(), testmode);
        p->imagemode(Pyro::CORNER);
        p->image(img, 0, 0);
        p->save(current_folder + filename);
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));

        delete p;
        delete img;
    }

    SECTION("1 channel")
    {
        std::string filename = "image_place_1ch.png";
        Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png")->convert(Pyro::GRAY);
        REQUIRE(img->channels() == 1);

        Pyro::Graphics *p = Pyro::creategraphics(img->width(), img->height(), testmode);
        p->imagemode(Pyro::CORNER);
        p->image(img, 0, 0);
        p->save(current_folder + filename);
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));

        delete p;
        delete img;
    }
}
