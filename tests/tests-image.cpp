#include <catch2/catch.hpp>

#include "pyro/pyroimage.h"
#include "pyro/pyroconstants.h"

unsigned int pack(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

TEST_CASE("Images are saved and loaded correctly", "[image]")
{
    SECTION("Load RGBA PNG")
    {
        Pyro::Image *img = Pyro::Image::load("../tests/TestPixels.png");

        uint32_t c = 0;

        // White opaque
        c = img->get(0, 0);
        REQUIRE(c == 0xffffffff);

        // Black opaque
        c = img->get(1, 0);
        REQUIRE(c == 0xff000000);

        // Transparent
        c = img->get(6, 0);
        REQUIRE(c == 0x00000000);

        // Red opaque
        c = img->get(7, 0);
        REQUIRE(c == 0xffff0000);

        // Blue Opaque
        c = img->get(0, 7);
        REQUIRE(c == 0xff0000ff);

        // Green opaque
        c = img->get(7, 7);
        REQUIRE(c == 0xff00ff00);
    }

    SECTION("saving an image and loading it gives same colors back")
    {
        Pyro::Image *img = Pyro::Image::create(1, 1);

        unsigned int pixel = 0xffff9911;

        unsigned int *pixels = img->load_pixels();
        REQUIRE(pixels != nullptr);

        pixels[0] = pixel;
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");
        pixels = img->load_pixels();
        REQUIRE(pixels != nullptr);

        REQUIRE(pixels[0] == pixel);

        delete img;
        delete img2;
        remove("test.png");
    }

    SECTION("Trying to load non-exsiting image gives nullptr")
    {
        Pyro::Image *img = Pyro::Image::load("askdjfhasd");
        REQUIRE(img == nullptr);
    }

    SECTION("Trying to load non-existing PNG gives nullptr")
    {
        Pyro::Image *img = Pyro::Image::loadPNG("asldkfjasdlf");
        REQUIRE(img == nullptr);
    }

    SECTION("Trying to load a file that is not PNG gives nullptr")
    {
        Pyro::Image *img = Pyro::Image::loadPNG("build.ninja");
        REQUIRE(img == nullptr);
    }

    SECTION("saving an image and loading it gives the same size")
    {
        Pyro::Image *img = Pyro::Image::create(4, 2);
        img->save("test.png");
        unsigned int pixel = 0xffff9911;

        unsigned int *pixels = img->load_pixels();
        REQUIRE(pixels != nullptr);

        pixels[0] = pixel;
        img->save("test.png");

        Pyro::Image *img2 = Pyro::Image::load("test.png");
        pixels = img->load_pixels();
        REQUIRE(pixels != nullptr);

        REQUIRE(img->width() == img2->width());
        REQUIRE(img->height() == img2->height());

        REQUIRE(pixels[0] == pixel);

        delete img;
        delete img2;
        remove("test.png");
    }

    SECTION("create image with gradient, test save and load")
    {
        SECTION("Create and save the image")
        {
            Pyro::Image *img = Pyro::Image::create(256, 5);

            unsigned int *pixels = img->load_pixels();
            REQUIRE(pixels != nullptr);

            for (unsigned int i = 0; i < 256; i++)
            {
                pixels[i] = 0xff000000 | i;
                pixels[256 + i] = 0xff000000 | i << 8;
                pixels[512 + i] = 0xff000000 | i << 16;
                pixels[768 + i] = 0xff000000 | i | i << 16 | i << 8;
                pixels[1024 + i] = i | i << 16 | i << 8 | i << 24;
            }

            img->save("gradient_test.png");
            delete img;
        }

        SECTION("Load the image back in")
        {
            Pyro::Image *img = Pyro::Image::load("gradient_test.png");

            unsigned int *pixels = img->load_pixels();
            REQUIRE(pixels != nullptr);

            for (unsigned int i = 0; i < 256; i++)
            {
                REQUIRE(pixels[i] == (0xff000000 | i));
                REQUIRE(pixels[256 + i] == (0xff000000 | i << 8));
                REQUIRE(pixels[512 + i] == (0xff000000 | i << 16));
                REQUIRE(pixels[768 + i] == (0xff000000 | i | i << 16 | i << 8));
                REQUIRE(pixels[1024 + i] == (i | i << 16 | i << 8 | i << 24));
            }
            delete img;

            //remove("gradient_test.png");
        }
    }

    SECTION("load an image from a different folder")
    {
        Pyro::Image *img = Pyro::Image::load("../tests/ducks.jpg");
        REQUIRE(img != nullptr);
        REQUIRE(img->width() == 512);
        REQUIRE(img->height() == 512);
    }
}

TEST_CASE("PNG files can be loaded and saved")
{
    SECTION("Load a test image")
    {
        Pyro::Image *img = Pyro::Image::loadPNG("../tests/TestPixels_RGB.png");
        REQUIRE(img != nullptr);
        REQUIRE(img->width() == 8);
        REQUIRE(img->height() == 8);
        REQUIRE(img->channels() == 3);

        SECTION("Check pixels")
        {
            REQUIRE(img->get(0, 0) == pack(255, 255, 255, 255));
            REQUIRE(img->get(img->width() - 1, 0) == pack(255, 0, 0, 255));
            REQUIRE(img->get(0, img->height() - 1) == pack(0, 0, 255, 255));
            REQUIRE(img->get(img->width() - 1, img->height() - 1) == pack(0, 255, 0, 255));
        }
    }

    SECTION("Save and load an image")
    {
        SECTION("Create and save")
        {
            Pyro::Image *img = Pyro::Image::create(256, 5);

            unsigned int *pixels = img->load_pixels();
            REQUIRE(pixels != nullptr);
            for (unsigned int i = 0; i < 256; i++)
            {
                pixels[i] = 0xff000000 | i;
                pixels[256 + i] = 0xff000000 | i << 8;
                pixels[512 + i] = 0xff000000 | i << 16;
                pixels[768 + i] = 0xff000000 | i | i << 16 | i << 8;
                pixels[1024 + i] = i | i << 16 | i << 8 | i << 24;
            }

            img->savePNG("savepng_gradient_test.png");
            delete img;
        }

        SECTION("Load the image back in")
        {
            Pyro::Image *img = Pyro::Image::loadPNG("savepng_gradient_test.png");

            unsigned int *pixels = img->load_pixels();
            REQUIRE(pixels != nullptr);

            for (unsigned int i = 0; i < 256; i++)
            {
                REQUIRE(pixels[i] == (0xff000000 | i));
                REQUIRE(pixels[256 + i] == (0xff000000 | i << 8));
                REQUIRE(pixels[512 + i] == (0xff000000 | i << 16));
                REQUIRE(pixels[768 + i] == (0xff000000 | i | i << 16 | i << 8));
                REQUIRE(pixels[1024 + i] == (i | i << 16 | i << 8 | i << 24));
            }
            delete img;
            //remove("savepng_gradient_test.png");
        }
    }
}

TEST_CASE("Images can be resized")
{
    SECTION("resize a created image")
    {
        Pyro::Image *img = Pyro::Image::create(64, 32);
        unsigned int pixelcolor = 0xff362d68;

        REQUIRE(img->width() == 64);
        REQUIRE(img->height() == 32);
        ((unsigned int *)img->get_data())[0] = pixelcolor;

        SECTION("scale down")
        {
            Pyro::Image *img2 = img->resize(32, 16);

            REQUIRE(img2->width() == 64 / 2);
            REQUIRE(img2->height() == 32 / 2);

            REQUIRE(((unsigned int *)img2->get_data())[0] == pixelcolor);

            delete img;
            delete img2;
        }

        SECTION("scale up")
        {
            Pyro::Image *img2 = img->resize(128, 64);

            REQUIRE(img2->width() == 64 * 2);
            REQUIRE(img2->height() == 32 * 2);
            REQUIRE(((unsigned int *)img2->get_data())[0] == pixelcolor);

            delete img;
            delete img2;
        }
    }

    SECTION("Resize a loaded image with different methods")
    {
        Pyro::Image *img = Pyro::Image::load("../tests/TestPixels_RGB.png");
        REQUIRE(img != nullptr);

        SECTION("Nearest neighbor scale down")
        {
            Pyro::Image *img2 = img->resize(500, 500, Pyro::NEAREST);

            REQUIRE(img2->width() == 500);
            REQUIRE(img2->height() == 500);

            REQUIRE((img2->get(0, 0) & 0xff000000) == 0xff000000);

            delete img2;
            delete img;
        }

        SECTION("Bilinear scale up")
        {
            Pyro::Image *img2 = img->resize(600, 600, Pyro::BILINEAR);

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);

            REQUIRE((img2->get(0, 0) & 0xff000000) == 0xff000000);

            delete img2;
            delete img;
        }

        SECTION("Bilinear scale down")
        {
            Pyro::Image *img2 = img->resize(400, 400, Pyro::BILINEAR);

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);

            REQUIRE((img2->get(0, 0) & 0xff000000) == 0xff000000);
            img2->save("bilinear_downscale.png");
            delete img2;
            delete img;
        }
    }
}

SCENARIO("Image can be created with a given size and depth")
{
    WHEN("Creating calling create with parameters 400, 300")
    {
        Pyro::Image *img = Pyro::Image::create(400, 300);
        THEN(" width is 400, height is 300 and depth is 4")
        {
            REQUIRE(img->width() == 400);
            REQUIRE(img->height() == 300);
            REQUIRE(img->channels() == 4);
        }
        delete img;
    }

    WHEN("Creating calling createimage with parameters 300, 400, 3")
    {
        Pyro::Image *img = Pyro::createimage(300, 400, 3);
        THEN(" width is 300, height is 400 and depth is 3")
        {
            REQUIRE(img->width() == 300);
            REQUIRE(img->height() == 400);
            REQUIRE(img->channels() == 3);
        }
        delete img;
    }

    WHEN("Creating calling createimage with parameters 180, 200, 4")
    {
        Pyro::Image *img = Pyro::createimage(180, 200, 4);
        THEN(" width is 180, height is 200 and depth is 4")
        {
            REQUIRE(img->width() == 180);
            REQUIRE(img->height() == 200);
            REQUIRE(img->channels() == 4);
        }
        delete img;
    }
}

TEST_CASE("Image can be created in different formats", "[image]")
{
    const char *filename = "__testimage_channels__.png";

    SECTION("Create single channel image")
    {
        Pyro::Image *img = Pyro::createimage(300, 200, Pyro::GRAY);
        REQUIRE(img->channels() == 1);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);

        img->save(filename);

        delete img;

        img = Pyro::Image::load(filename);
        REQUIRE(img->channels() == 1);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);
    }

    SECTION("Create RGB image")
    {
        Pyro::Image *img = Pyro::createimage(300, 200, Pyro::RGB);
        REQUIRE(img->channels() == 3);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);

        img->save(filename);

        delete img;

        img = Pyro::Image::load(filename);
        REQUIRE(img->channels() == 3);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);
    }

    SECTION("Create ARGB image")
    {
        Pyro::Image *img = Pyro::createimage(300, 200, Pyro::ARGB);
        REQUIRE(img->channels() == 4);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);

        img->save(filename);

        delete img;

        img = Pyro::Image::load(filename);
        REQUIRE(img->channels() == 4);
        REQUIRE(img->width() == 300);
        REQUIRE(img->height() == 200);
    }
    remove(filename);
}

SCENARIO("Images can be resized proportionally", "[image]")
{
    GIVEN("An image that is 400x300")
    {
        Pyro::Image *img = Pyro::Image::create(400, 300);
        WHEN("Scaling down, setting only width to 200")
        {
            Pyro::Image *img2 = img->resize(200, 0);

            THEN(" width and height will be 200 x 150 ")
            {
                REQUIRE(img2->width() == 200);
                REQUIRE(img2->height() == 150);
            }

            delete img2;
            delete img;
        }

        WHEN("Scaling down, setting only height to 150")
        {
            Pyro::Image *img2 = img->resize(0, 150);

            THEN(" width and height will be 200 x 150")
            {
                REQUIRE(img2->width() == 200);
                REQUIRE(img2->height() == 150);
            }
            delete img2;
            delete img;
        }

        WHEN("scaling up, settings only width to 800")
        {
            Pyro::Image *img2 = img->resize(800, 0);

            THEN(" width and height will be 800 x 600")
            {
                REQUIRE(img2->width() == 800);
                REQUIRE(img2->height() == 600);
            }

            delete img2;
            delete img;
        }

        WHEN("Scaling up, setting only height to 600")
        {
            Pyro::Image *img2 = img->resize(0, 600);

            THEN(" width and height will be 800 x 600")
            {
                REQUIRE(img2->width() == 800);
                REQUIRE(img2->height() == 600);
            }
            delete img2;
            delete img;
        }
    }
}
