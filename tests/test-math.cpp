#include <catch2/catch.hpp>

#include <pyroconstants.h>
#include <pyromath.h>



TEST_CASE("Random generation seems sane", "[random]") {
    SECTION("Setting random seed resets sequence") {
        unsigned int seed = 0;
        Pyro::randomseed(seed);

        unsigned int first = Pyro::random(100u);

        for(unsigned int i = 0; i < 100; i++) {
            Pyro::randomseed(seed);
            unsigned int n = Pyro::random(100u);
            REQUIRE(n == first);
        }
    }

    SECTION("Varying random seed gives different sequences") {
        unsigned int firsts[100] = { 0 };
        for(unsigned int i = 0; i < 100; i++) {
            Pyro::randomseed(i);
            unsigned int v = Pyro::random(100u);
            firsts[v]++;
        }

        for(unsigned int i = 0; i < 100; i++) {
            INFO("The number is " << i);
            REQUIRE(firsts[i] < 99);
        }
    }
}

TEST_CASE("Values can be...") {
    SECTION("constrained") {
        REQUIRE(Pyro::constrain(0, 10, 20) == 10);
        REQUIRE(Pyro::constrain(30, 10, 20) == 20);
    }
}

TEST_CASE("We have functions that can...") {
    SECTION("give min value") {
        REQUIRE(Pyro::min(0, 10) == 0);
        REQUIRE(Pyro::min(-3, 5) == -3);
        REQUIRE(Pyro::min(53, 3) == 3);
        REQUIRE(Pyro::min(23, -34) == -34);
    }

    SECTION("give max value") {
        REQUIRE(Pyro::max(0, 10) == 10);
        REQUIRE(Pyro::max(-3, 5) == 5);
        REQUIRE(Pyro::max(53, 3) == 53);
        REQUIRE(Pyro::max(23, -34) == 23);
    }

}

SCENARIO("Degrees can be converted to radians and back") {
    GIVEN("0 degrees") {
        float d = 0.0;
        float r = Pyro::radians(d);

        THEN("is converted to 0 radians") {
            REQUIRE(r == 0);
        }

        THEN("is converted back to 0 degrees") {
            REQUIRE(Pyro::degrees(r) == d);
        }
    }
    GIVEN("90 degrees") {
        float d = 90.0;
        float r = Pyro::radians(d);
        THEN("is converted to Pi / 2 radians") {
            REQUIRE(r == Approx(Pyro::PI / 2.0));
        }

        THEN("is converted back to 90 degrees") {
            REQUIRE(Pyro::degrees(r) == d);
        }
    }

    GIVEN("180 degrees") {
        float d = 180.0;
        float r = Pyro::radians(d);
        THEN("is converted to Pi radians") {
            REQUIRE(r == Approx(Pyro::PI));
        }

        THEN("is converted back to 180 degrees") {
            REQUIRE(Pyro::degrees(r) == d);
        }
    }

    GIVEN("270 degrees") {
        float d = 270.0;
        float r = Pyro::radians(d);
        THEN("is converted to 1.5 * Pi radians") {
            REQUIRE(r == Approx(Pyro::PI + Pyro::PI / 2.0));
        }

        THEN("is converted back to 270 degrees") {
            REQUIRE(Pyro::degrees(r) == d);
        }
    }

}