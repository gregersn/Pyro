#include <catch2/catch.hpp>

#include "pyro/graphics.h"

TEST_CASE("Graphics objects can be initialized with different engines", "[graphics]")
{
    SECTION("Create a graphics object with default engine")
    {
        Pyro::Graphics *graphics = Pyro::Graphics::create(400, 300);

        REQUIRE(graphics != nullptr);
        REQUIRE(graphics->width() == 400);
        REQUIRE(graphics->height() == 300);
    }

    SECTION("Create a graphics object with cairo engine")
    {
        Pyro::Graphics *graphics = Pyro::Graphics::create(400, 300, Pyro::GraphicsMode::CAIRO);

        REQUIRE(graphics != nullptr);
        REQUIRE(graphics->width() == 400);
        REQUIRE(graphics->height() == 300);
    }

    SECTION("Create a graphics object with 3d engine")
    {
        Pyro::Graphics *graphics = Pyro::Graphics::create(400, 300, Pyro::GraphicsMode::PYRO3D);

        REQUIRE(graphics != nullptr);
        REQUIRE(graphics->width() == 400);
        REQUIRE(graphics->height() == 300);
    }
}