#include "catch.hpp"

#include <pyroimage.h>

TEST_CASE( "Images are saved and loaded correctly", "[image]") {
    SECTION("saving an image and loading it gives same colors back") {
        Pyro::Image *img = Pyro::Image::create(1, 1);

        unsigned int pixel = 0xffff9911;
        
        unsigned int *pixels = img->load_pixels();
        pixels[0] = pixel;
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");
        pixels = img->load_pixels();

        REQUIRE(pixels[0] == pixel);

        delete img;
        delete img2;
    }

    SECTION("saving an image and loading it gives the same size") {
        Pyro::Image *img = Pyro::Image::create(4, 2);
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");

        REQUIRE(img->width() == img2->width());
        REQUIRE(img->height() == img2->height());

        delete img;
        delete img2;
    }
}
