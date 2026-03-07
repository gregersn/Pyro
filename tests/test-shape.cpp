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

TEST_CASE("Check bsplinepoint")
{
    std::vector points = std::vector{
        Pyro::Vector(.0, .0),
        Pyro::Vector(1.0, .0),
        Pyro::Vector(2.0, .0),
        Pyro::Vector(3.0, .0),
    };

    SECTION("Check splineknot creation")
    {
        std::vector<int> knots{};
        std::vector<int> expected_knots = {0, 0, 0, 1, 2, 2, 2};
        Pyro::splineknots(&knots, 4, 2, true);
        REQUIRE(knots == expected_knots);
    }

    SECTION("Check bsplinepointvalue with knots")
    {
        std::vector knots = std::vector{0, 0, 0, 1, 2, 2, 2};

        Pyro::Vector res = Pyro::bsplinepoint(points, 0.0, 2, true, knots);
        REQUIRE(res.x == 0.0f);

        res = Pyro::bsplinepoint(points, 0.5, 2, true);
        REQUIRE(res.x == 1.5f);

        res = Pyro::bsplinepoint(points, 1.0, 2, true);
        REQUIRE(res.x == 3.0f);
    }

    SECTION("Check bsplinepointvalue without knots")
    {

        Pyro::Vector res = Pyro::bsplinepoint(points, 0.0, 2, true);
        REQUIRE(res.x == 0.0f);

        res = Pyro::bsplinepoint(points, 0.5, 2, true);
        REQUIRE(res.x == 1.5f);

        res = Pyro::bsplinepoint(points, 1.0, 2, true);
        REQUIRE(res.x == 3.0f);
    }
}
