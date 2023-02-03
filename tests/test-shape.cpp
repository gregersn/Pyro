#include <catch2/catch_all.hpp>

#include "pyro/shape.h"

TEST_CASE("Check if a point is inside a shape",
          "[shape]")
{
    SECTION("Point inside square")
    {
        Pyro::Shape shape = Pyro::Shape();

        shape.begin();
        shape.vertex(0, 0);
        shape.vertex(10, 0);
        shape.vertex(10, 10);
        shape.vertex(0, 10);

        REQUIRE(shape.is_point_in_path(5, 5));
    }

    SECTION("Point outside square")
    {
        Pyro::Shape shape = Pyro::Shape();

        shape.begin();
        shape.vertex(0, 0);
        shape.vertex(10, 0);
        shape.vertex(10, 10);
        shape.vertex(0, 10);

        REQUIRE(!shape.is_point_in_path(15, 51));
    }
}
