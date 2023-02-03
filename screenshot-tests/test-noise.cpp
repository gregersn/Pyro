#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/noise.h"
#include "pyro/pyro.h"

TEST_CASE("Test noise")
{
    std::string filename = "noise_2d.png";
    Pyro::Image *img = Pyro::Image::create(128, 128);
    unsigned int *pixels = img->load_pixels();

    for (uint y = 0; y < 128; y++)
    {
        for (uint x = 0; x < 128; x++)
        {
            Pyro::noisedetail(1, 1.0);
            double v = Pyro::noise(x * 0.1, y * 0.1);
            REQUIRE(v >= 0.0);
            pixels[y * 128 + x] = 0xff000000 | (uint8_t)((v)*255);
        }
    }

    img->save(current_folder + filename);
    delete img;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}
