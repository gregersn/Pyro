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

    SECTION("load an image from a different folder") {
        Pyro::Image *img = Pyro::Image::load("../tests/Lenna.png");
        REQUIRE(img->width() == 512);
        REQUIRE(img->height() == 512);
    }
}

TEST_CASE("Images can be resized") {
    SECTION("resize a created image") {
        Pyro::Image *img = Pyro::Image::create(64, 32);
        unsigned int pixelcolor = 0xff362d68;

        REQUIRE(img->width() == 64);
        REQUIRE(img->height() == 32);
        ((unsigned int *)img->get_data())[0] = pixelcolor;

        SECTION("scale down") {
            Pyro::Image *img2 = img->resize(32, 16);

            REQUIRE(img2->width() == 64 / 2);
            REQUIRE(img2->height() == 32 / 2);
            REQUIRE(((unsigned int *)img2->get_data())[0] == pixelcolor);

            delete img;
            delete img2;
        }

        SECTION("scale up") {
            Pyro::Image *img2 = img->resize(128, 64);

            REQUIRE(img2->width() == 64 * 2);
            REQUIRE(img2->height() == 32 * 2);
            REQUIRE(((unsigned int *)img2->get_data())[0] == pixelcolor);

            delete img;
            delete img2;
        }
    }

    SECTION("Resize a loaded image with different methods") {
        Pyro::Image *img = Pyro::Image::load("../tests/Lenna.png");
        
        SECTION("Nearest neighbor scale down") {
            Pyro::Image *img2 = img->resize(500, 500, Pyro::NEAREST);

            REQUIRE(img2->width() == 500);
            REQUIRE(img2->height() == 500);
            uint32_t *pixels = img2->load_pixels();
            REQUIRE((pixels[0]&0xff000000) == 0xff000000);

            delete img2;
            delete img;
        }      

        SECTION("Bilinear scale up") {
            Pyro::Image *img2 = img->resize(600, 600, Pyro::BILINEAR);

            REQUIRE(img2->width() == 600);
            REQUIRE(img2->height() == 600);
            uint32_t *pixels = img2->load_pixels();
            REQUIRE((pixels[0]&0xff000000) == 0xff000000);

            delete img2;
            delete img;
        }

        SECTION("Bilinear scale down") {
            Pyro::Image *img2 = img->resize(400, 400, Pyro::BILINEAR);

            REQUIRE(img2->width() == 400);
            REQUIRE(img2->height() == 400);
            uint32_t *pixels = img2->load_pixels();
            REQUIRE((pixels[0]&0xff000000) == 0xff000000);
            delete img2;
            delete img;
        }

    }
}

SCENARIO("Image can be created with a given size and depth") {
    WHEN("Creating calling create with parameters 400, 300") {
        Pyro::Image *img = Pyro::Image::create(400, 300);
        THEN(" width is 400, height is 300 and depth is 4") {
            REQUIRE(img->width() == 400);
            REQUIRE(img->height() == 300);
            REQUIRE(img->bpp == 4);
        }
        delete img;
    }

    WHEN("Creating calling createimage with parameters 300, 400, 3") {
        Pyro::Image *img = Pyro::createimage(300, 400, 3);
        THEN(" width is 300, height is 400 and depth is 3") {
            REQUIRE(img->width() == 300);
            REQUIRE(img->height() == 400);
            REQUIRE(img->bpp == 3);
        }
        delete img;
    }

    WHEN("Creating calling createimage with parameters 180, 200, 4") {
        Pyro::Image *img = Pyro::createimage(180, 200, 4);
        THEN(" width is 180, height is 200 and depth is 4") {
            REQUIRE(img->width() == 180);
            REQUIRE(img->height() == 200);
            REQUIRE(img->bpp == 4);
        }
        delete img;
    }
}

SCENARIO("Images can be resized proportionally", "[image]") {
    GIVEN("An image that is 400x300") {
        Pyro::Image *img = Pyro::Image::create(400, 300);
        WHEN("Scaling down, setting only width to 200") {
            Pyro::Image *img2 = img->resize(200, 0);

            THEN(" width and height will be 200 x 150 ") {
                REQUIRE(img2->width() == 200);
                REQUIRE(img2->height() == 150);
            }

            delete img2;
            delete img;
        }

        WHEN("Scaling down, setting only height to 150") {
            Pyro::Image *img2 = img->resize(0, 150);

            THEN(" width and height will be 200 x 150") {
                REQUIRE(img2->width() == 200);
                REQUIRE(img2->height() == 150);
            }
            delete img2;
            delete img;
        }

        WHEN("scaling up, settings only width to 800") {
            Pyro::Image *img2 = img->resize(800, 0);

            THEN(" width and height will be 800 x 600") {
                REQUIRE(img2->width() == 800);
                REQUIRE(img2->height() == 600);
            }

            delete img2;
            delete img;
        }

        WHEN ("Scaling up, setting only height to 600") {
            Pyro::Image *img2 = img->resize(0, 600);

            THEN(" width and height will be 800 x 600") {
                REQUIRE(img2->width() == 800);
                REQUIRE(img2->height() == 600);
            }            
            delete img2;
            delete img;
        }
    }
}
