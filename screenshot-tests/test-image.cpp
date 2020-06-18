#include <catch2/catch.hpp>
#include "test-settings.h"

#include "pyro/pyroimage.h"
#include "pyro/pyroconstants.h"

SCENARIO("Image can be converted to different depths")
{
    GIVEN("An image with 4 channels")
    {
        Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");

        WHEN("Converted to 3 channels")
        {
            std::string filename = "image_convert_4ch_to_3ch.png";
            Pyro::Image *img2 = img->convert(Pyro::RGB);
            THEN(" the new depth will be 3")
            {
                REQUIRE(img2->channels() == 3);
                img2->save(current_folder + filename);
                delete img2;

                CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
            }
        }

        WHEN("Converted to 1 channel")
        {
            std::string filename = "image_convert_4ch_to_1ch.png";
            Pyro::Image *img2 = img->convert(Pyro::GRAY);
            THEN(" the new depth will be 1")
            {
                REQUIRE(img2->channels() == 1);
                img2->save(current_folder + filename);
                delete img2;

                CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
            }
        }

        delete img;
    }

    GIVEN("An image with 3 channels")
    {
        Pyro::Image *img = Pyro::createimage(32, 32, 3);

        WHEN("Converted to 4 channels")
        {
            Pyro::Image *img2 = img->convert(Pyro::ARGB);
            THEN(" the new depth will be 4")
            {
                REQUIRE(img2->channels() == 4);
            }
        }

        WHEN("Converted to 1 channel")
        {
            Pyro::Image *img2 = img->convert(Pyro::GRAY);
            THEN(" the new depth will be 1")
            {
                REQUIRE(img2->channels() == 1);
            }
        }
    }

    GIVEN("An image with 1 channel")
    {
        Pyro::Image *img = Pyro::createimage(32, 32, 1);

        WHEN("Converted to 4 channels")
        {
            Pyro::Image *img2 = img->convert(Pyro::ARGB);
            THEN(" the new depth will be 4")
            {
                REQUIRE(img2->channels() == 4);
            }
        }

        WHEN("Converted to 3 channel")
        {
            Pyro::Image *img2 = img->convert(Pyro::RGB);
            THEN(" the new depth will be 3")
            {
                REQUIRE(img2->channels() == 3);
            }
        }
    }
}