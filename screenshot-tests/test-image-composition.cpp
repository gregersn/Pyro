#include <catch2/catch.hpp>
#include "test-settings.h"

#include "pyro/image.h"

TEST_CASE("images can be composited")
{
    SECTION("a over b")
    {
        std::string filename = "image_a_over_b.png";
        Pyro::Image *a = Pyro::Image::load(actual_folder + "test_image.png");
        Pyro::Image *b = a->resize(a->width() / 2, a->height() / 2);

        a->set(a->width() / 2, a->height() / 2, b);
        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }

    SECTION("a over b with alpha")
    {
        std::string filename = "image_a_over_b_with_alpha.png";
        Pyro::Image *a = Pyro::Image::load(actual_folder + "test_image.png");
        Pyro::Image *b = Pyro::Image::create(a->width(), a->height());

        unsigned char *data = b->load_bytes();
        for (uint y = 0; y < b->height(); y++)
        {
            for (uint x = 0; x < b->width(); x++)
            {
                data[y * b->width() * b->channels() + x * b->channels() + 0] = 0;
                data[y * b->width() * b->channels() + x * b->channels() + 1] = 0;
                data[y * b->width() * b->channels() + x * b->channels() + 2] = 0;

                data[y * b->width() * b->channels() + x * b->channels() + 3] = x * 255 / b->width();
            }
        }
        b->update_pixels();

        a->set(0, 0, b);
        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
}

TEST_CASE("Images can mask each other")
{
    SECTION("Use single channel image as mask")
    {
        std::string filename = "image_single_channel_mask.png";

        Pyro::Image *a = Pyro::Image::load(actual_folder + "test_image.png");
        Pyro::Image *b = Pyro::createimage(a->width(), a->height(), Pyro::GRAY);

        u_int32_t *mask_pixels = b->load_pixels();
        for (uint y = 0; y < b->height(); y++)
        {
            for (uint x = 0; x < b->width(); x++)
            {
                mask_pixels[y * b->width() + x] = x | y;
            }
        }

        a->mask(b);
        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
}