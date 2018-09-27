#include "catch.hpp"

#include <pyrovector.h>

TEST_CASE( "Vectors can be added", "[vector]") {
    Pyro::Vector a = Pyro::Vector(0.0f, 1.0f);
    Pyro::Vector b = Pyro::Vector(1.0f, 0.0f);

    REQUIRE(a.x() == 0.0f);
    REQUIRE(a.y() == 1.0f);
    REQUIRE(b.x() == 1.0f);
    REQUIRE(b.y() == 0.0f);

    SECTION("add b to a") {
        Pyro::Vector c = a + b;

        REQUIRE(c.x() == 1.0f);
        REQUIRE(c.y() == 1.0f);
    }

    SECTION("add a to b") {
        Pyro::Vector c = b + a;

        REQUIRE(c.x() == 1.0f);
        REQUIRE(c.y() == 1.0f);
    }

    SECTION("Adding is commutative") {
        REQUIRE(a + b == b + a);
    }
}

TEST_CASE( "Vectors can be subtracted", "[vector]") {
    Pyro::Vector a = Pyro::Vector(0.0f, 1.0f);
    Pyro::Vector b = Pyro::Vector(1.0f, 0.0f);

    REQUIRE(a.x() == 0.0f);
    REQUIRE(a.y() == 1.0f);
    REQUIRE(b.x() == 1.0f);
    REQUIRE(b.y() == 0.0f);

    SECTION("subtract b from a") {
        Pyro::Vector c = a - b;
        REQUIRE(c.x() == -1.0f);
        REQUIRE(c.y() == 1.0f);
    }

    SECTION("subtract a from b") {
        Pyro::Vector c = b - a;
        REQUIRE(c.x() == 1.0f);
        REQUIRE(c.y() == -1.0f);
    }

    SECTION("Subtraction is not commutative") {
        REQUIRE_FALSE(a - b == b - a);
    }
}

TEST_CASE( "Vectors have magnitude", "[vector]") {
    REQUIRE((Pyro::Vector(1.0f, 0.0f)).mag() == 1.0f);
    REQUIRE((Pyro::Vector(0.0f, 1.0f)).mag() == 1.0f);

    REQUIRE((Pyro::Vector(3.0f, 4.0f)).mag() == 5.0f);
    REQUIRE((Pyro::Vector(5.0f, 12.0f)).mag() == 13.0f);

    REQUIRE((Pyro::Vector(8.0f, 15.0f)).mag() == 17.0f);
    REQUIRE((Pyro::Vector(7.0f, 24.0f)).mag() == 25.0f);


}