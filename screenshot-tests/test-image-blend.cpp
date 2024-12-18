#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include "pyro/image.h"

TEST_CASE("Blend modes")
{
    SECTION("BLEND")
    {
        std::filesystem::path filename = "image_blend_BLEND.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BLEND);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("ADD")
    {
        std::filesystem::path filename = "image_blend_ADD.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::ADD);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("SUBTRACT")
    {
        std::filesystem::path filename = "image_blend_SUBTRACT.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::SUBTRACT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("LIGHTEST")
    {
        std::filesystem::path filename = "image_blend_LIGHTEST.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::LIGHTEST);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("DARKEST")
    {
        std::filesystem::path filename = "image_blend_DARKEST.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::DARKEST);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("DIFFERENCE")
    {
        std::filesystem::path filename = "image_blend_DIFFERENCE.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::DIFFERENCE);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("EXCLUSION")
    {
        std::filesystem::path filename = "image_blend_EXCLUSION.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::EXCLUSION);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("MULTIPLY")
    {
        std::filesystem::path filename = "image_blend_MULTIPLY.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::MULTIPLY);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("SCREEN")
    {
        std::filesystem::path filename = "image_blend_SCREEN.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::SCREEN);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("OVERLAY")
    {
        std::filesystem::path filename = "image_blend_OVERLAY.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::OVERLAY);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("HARD_LIGHT")
    {
        std::filesystem::path filename = "image_blend_HARD_LIGHT.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::HARD_LIGHT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("SOFT_LIGHT")
    {
        std::filesystem::path filename = "image_blend_SOFT_LIGHT.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::SOFT_LIGHT);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("DODGE")
    {
        std::filesystem::path filename = "image_blend_DODGE.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::DODGE);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
    SECTION("BURN")
    {
        std::filesystem::path filename = "image_blend_BURN.png";

        Pyro::Image *a = Pyro::Image::load("./tests/ducks.jpg");
        Pyro::Image *b = Pyro::Image::load("./tests/alley.jpg");

        a->blend(b, 0, 0, 150, 512, 0, 0, 150, 512, Pyro::BURN);
        a->blend(b, 0, 0, 150, 512, 512 - 150, 0, 150, 512, Pyro::BLEND);

        a->save(current_folder / filename);

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
}
