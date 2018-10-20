#include "catch.hpp"

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

    SECTION("Getting random numbers is a varying sequence") {
        unsigned int seed = 0;
        Pyro::randomseed(seed);
        unsigned int collisions = 0;
        unsigned int prev = Pyro::random(100u);
        for(unsigned int i =0; i < 100; i++) {
            unsigned int cur = Pyro::random(100u);
            if(cur == prev) {
                collisions++;
            }
            prev = cur;
        }
        INFO("Number of equals in a sequence" << collisions);
        CHECK(collisions < 2);
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