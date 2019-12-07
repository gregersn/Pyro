#include <catch2/catch.hpp>

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
        unsigned int pixel = 0xffff9911;
        
        unsigned int *pixels = img->load_pixels();
        pixels[0] = pixel;
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");
        pixels = img->load_pixels();

        REQUIRE(img->width() == img2->width());
        REQUIRE(img->height() == img2->height());

        REQUIRE(pixels[0] == pixel);


        delete img;
        delete img2;
    }

    SECTION("create image with gradient, test save and load") {
        Pyro::Image *img = Pyro::Image::create(256, 5);

        unsigned int *pixels = img->load_pixels();
        for(unsigned int i = 0; i < 256; i++) {
            pixels[i] = 0xff000000 | i;
            pixels[256 + i] = 0xff000000 | i << 8;
            pixels[512 + i] = 0xff000000 | i << 16;
            pixels[768 + i] = 0xff000000 | i | i << 16 | i << 8;
            pixels[1024 + i] = i | i << 16 | i << 8 | i << 24;
            
        }

        img->save("gradient_test.png");
        delete img;

        img = Pyro::Image::load("gradient_test.png");

        pixels = img->load_pixels();
        for(unsigned int i = 0; i < 256; i++) {
            REQUIRE(pixels[i] == (0xff000000 | i));
            REQUIRE(pixels[256 + i] == (0xff000000 | i << 8));
            REQUIRE(pixels[512 + i] == (0xff000000 | i << 16));
            REQUIRE(pixels[768 + i] == (0xff000000 | i | i << 16 | i << 8));
            REQUIRE(pixels[1024 + i] == (i | i << 16 | i << 8 | i << 24));
        }
        delete img;
    }
}
