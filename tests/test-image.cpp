#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <pyroimage.h>

TEST_CASE( "Images are saved and loaded correctly", "[image]") {
    SECTION("saveing an image and loading it gives same result") {
        Pyro::Image *img = Pyro::Image::create(1, 1);

        unsigned int pixel = 0xffff0000;
        
        unsigned int *pixels = img->load_pixels();
        pixels[0] = pixel;
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");
        pixels = img->load_pixels();

        REQUIRE(pixels[0] == pixel);

        delete img;
        delete img2;
    }
}
