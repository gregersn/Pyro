#include <catch2/catch.hpp>

#include "pyro/pyro.h"

TEST_CASE("Noise values", "[noise]")
{
    SECTION("Values are between 0 and 1")
    {
        for (uint16_t y = 0; y < 1000; y++)
        {
            for (uint16_t x = 0; x < 1000; x++)
            {
                double value = Pyro::noise(x / 30.0, y / 30.0);
                REQUIRE(value >= 0.0);
                REQUIRE(value <= 1.0);
            }
        }
    }

    SECTION("Values can be reproduced")
    {
        Pyro::Vector field[2][10];
        for (uint16_t y = 0; y < 2; y++)
        {
            for (uint16_t x = 0; x < 10; x++)
            {
                double angle = Pyro::noise(x / 50.0, y / 50.0, 10.0) * Pyro::TWO_PI;
                double length = Pyro::noise(x / 10.0 + 40000, y / 10.0 + 40000, 10.0);

                field[y][x] = Pyro::Vector(angle, length);
            }
        }

        for (uint16_t y = 0; y < 2; y++)
        {
            for (uint16_t x = 0; x < 10; x++)
            {
                double angle = Pyro::noise(x / 50.0, y / 50.0, 10.0) * Pyro::TWO_PI;
                double length = Pyro::noise(x / 10.0 + 40000, y / 10.0 + 40000, 10.0);

                Pyro::Vector v = Pyro::Vector(angle, length);

                REQUIRE(field[y][x].x == v.x);
                REQUIRE(field[y][x].y == v.y);
            }
        }
    }
}