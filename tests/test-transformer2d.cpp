#include <catch2/catch_all.hpp>
#include "pyro/transformer.h"
#include "pyro/pyro.h"

using namespace Pyro;
using Catch::Approx;

TEST_CASE("Basic transformations", "[transformer]")
{
    SECTION("Initializes to identity")
    {
        Transformer2D t = Transformer2D();
        REQUIRE(t.getcurrent()(0, 0) == 1);
        REQUIRE(t.getcurrent()(0, 1) == 0);
        REQUIRE(t.getcurrent()(0, 2) == 0);

        REQUIRE(t.getcurrent()(1, 0) == 0);
        REQUIRE(t.getcurrent()(1, 1) == 1);
        REQUIRE(t.getcurrent()(1, 2) == 0);

        REQUIRE(t.getcurrent()(2, 0) == 0);
        REQUIRE(t.getcurrent()(2, 1) == 0);
        REQUIRE(t.getcurrent()(2, 2) == 1);
    }

    SECTION("Translate to position")
    {
        Transformer2D t = Transformer2D();
        t.translate(2, 4);
        REQUIRE(t.getcurrent()(0, 0) == 1);
        REQUIRE(t.getcurrent()(0, 1) == 0);
        REQUIRE(t.getcurrent()(0, 2) == 2);

        REQUIRE(t.getcurrent()(1, 0) == 0);
        REQUIRE(t.getcurrent()(1, 1) == 1);
        REQUIRE(t.getcurrent()(1, 2) == 4);

        REQUIRE(t.getcurrent()(2, 0) == 0);
        REQUIRE(t.getcurrent()(2, 1) == 0);
        REQUIRE(t.getcurrent()(2, 2) == 1);
    }

    SECTION("Rotate")
    {
        Transformer2D t = Transformer2D();
        t.rotate(Pyro::HALF_PI);
        REQUIRE(t.getcurrent()(0, 0) == Approx(0).margin(.0001));
        REQUIRE(t.getcurrent()(0, 1) == Approx(-1).margin(.0001));
        REQUIRE(t.getcurrent()(0, 2) == 0);

        REQUIRE(t.getcurrent()(1, 0) == Approx(1).margin(.0001));
        REQUIRE(t.getcurrent()(1, 1) == Approx(0).margin(.0001));
        REQUIRE(t.getcurrent()(1, 2) == 0);

        REQUIRE(t.getcurrent()(2, 0) == 0);
        REQUIRE(t.getcurrent()(2, 1) == 0);
        REQUIRE(t.getcurrent()(2, 2) == 1);
    }

    SECTION("Scale")
    {
        Transformer2D t = Transformer2D();
        t.scale(1.5, 2.3);
        REQUIRE(t.getcurrent()(0, 0) == 1.5);
        REQUIRE(t.getcurrent()(0, 1) == 0);
        REQUIRE(t.getcurrent()(0, 2) == 0);

        REQUIRE(t.getcurrent()(1, 0) == 0);
        REQUIRE(t.getcurrent()(1, 1) == Approx(2.3));
        REQUIRE(t.getcurrent()(1, 2) == 0);

        REQUIRE(t.getcurrent()(2, 0) == 0);
        REQUIRE(t.getcurrent()(2, 1) == 0);
        REQUIRE(t.getcurrent()(2, 2) == 1);
    }
}

TEST_CASE("Apply to Pyro Vector", "[transform]")
{
    SECTION("Translate")
    {
        Transformer2D t = Transformer2D();
        t.translate(2, 3);

        Pyro::Vector v = Pyro::Vector(0.0f, 0.0f);

        v = t.apply(v);

        REQUIRE(v.x == 2);
        REQUIRE(v.y == 3);
    }

    SECTION("Rotate")
    {
        Transformer2D t = Transformer2D();
        t.rotate(Pyro::PI);

        Pyro::Vector v = Pyro::Vector(1.0f, 0.0f);

        v = t.apply(v);

        REQUIRE(v.x == -1.0);
        REQUIRE(v.y == Approx(0.0).margin(0.00001));
    }
}

TEST_CASE("Get screen coordinates", "[transform]")
{
    SECTION("Simple translate")
    {
        Transformer2D t = Transformer2D();

        REQUIRE(t.screen_x(0, 0, 0) == Approx(0.0));
        REQUIRE(t.screen_x(0, 10, 0) == Approx(0.0));
        REQUIRE(t.screen_x(0, 0, 10) == Approx(0.0));
        REQUIRE(t.screen_x(4, 0, 0) == Approx(4.0));

        REQUIRE(t.screen_y(0, 0, 0) == Approx(0.0));
        REQUIRE(t.screen_y(0, 10, 0) == Approx(10.0));
        REQUIRE(t.screen_y(0, 0, 10) == Approx(0.0));
        REQUIRE(t.screen_y(4, 0, 0) == Approx(0.0));
    }
}

TEST_CASE("Get screen coordinates from Pyro", "[transform]")
{
    SECTION("Simple translate")
    {
        Pyro::size(640, 480);

        REQUIRE(Pyro::screen_x(0, 0, 0) == Approx(0.0));
        REQUIRE(Pyro::screen_x(0, 10, 0) == Approx(0.0));
        REQUIRE(Pyro::screen_x(0, 0, 10) == Approx(0.0));
        REQUIRE(Pyro::screen_x(4, 0, 0) == Approx(4.0));

        REQUIRE(Pyro::screen_y(0, 0, 0) == Approx(0.0));
        REQUIRE(Pyro::screen_y(0, 10, 0) == Approx(10.0));
        REQUIRE(Pyro::screen_y(0, 0, 10) == Approx(0.0));
        REQUIRE(Pyro::screen_y(4, 0, 0) == Approx(0.0));
    }
}

TEST_CASE("Push and pop matrix", "[transform]")
{
    SECTION("Push/pop")
    {
        Transformer2D t = Transformer2D();
        t.pushmatrix();
        t.translate(2, 4);
        t.popmatrix();

        REQUIRE(t.getcurrent()(0, 0) == 1);
        REQUIRE(t.getcurrent()(0, 1) == 0);
        REQUIRE(t.getcurrent()(0, 2) == 0);

        REQUIRE(t.getcurrent()(1, 0) == 0);
        REQUIRE(t.getcurrent()(1, 1) == 1);
        REQUIRE(t.getcurrent()(1, 2) == 0);

        REQUIRE(t.getcurrent()(2, 0) == 0);
        REQUIRE(t.getcurrent()(2, 1) == 0);
        REQUIRE(t.getcurrent()(2, 2) == 1);
    }
}
