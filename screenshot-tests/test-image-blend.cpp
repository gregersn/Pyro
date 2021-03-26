#include <catch2/catch.hpp>
#include "test-settings.h"

#include "pyro/pyroimage.h"

TEST_CASE("Blend modes")
{
    SECTION("BLEND")
    {
        std::string filename = "image_blend_BLEND.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::BLEND);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("ADD")
    {
        std::string filename = "image_blend_ADD.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::ADD);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("SUBTRACT")
    {
        std::string filename = "image_blend_SUBTRACT.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::SUBTRACT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("LIGHTEST")
    {
        std::string filename = "image_blend_LIGHTEST.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::LIGHTEST);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("DARKEST")
    {
        std::string filename = "image_blend_DARKEST.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::DARKEST);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("DIFFERENCE")
    {
        std::string filename = "image_blend_DIFFERENCE.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::DIFFERENCE);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("EXCLUSION")
    {
        std::string filename = "image_blend_EXCLUSION.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::EXCLUSION);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("MULTIPLY")
    {
        std::string filename = "image_blend_MULTIPLY.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::MULTIPLY);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("SCREEN")
    {
        std::string filename = "image_blend_SCREEN.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::SCREEN);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("OVERLAY")
    {
        std::string filename = "image_blend_OVERLAY.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::OVERLAY);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("HARD_LIGHT")
    {
        std::string filename = "image_blend_HARD_LIGHT.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::HARD_LIGHT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("SOFT_LIGHT")
    {
        std::string filename = "image_blend_SOFT_LIGHT.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::SOFT_LIGHT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("DODGE")
    {
        std::string filename = "image_blend_DODGE.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::DODGE);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
    SECTION("BURN")
    {
        std::string filename = "image_blend_BURN.png";

        Pyro::Image *a = Pyro::Image::load("../tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("../tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLENDMODE::BURN);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLENDMODE::BLEND);

        a->save(current_folder + filename);

        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }
}