#include <catch2/catch.hpp>

#include <pyro/pyro.h>

TEST_CASE("Noise values", "[noise]") {
    SECTION("Values are between 0 and 1") {
        for(uint16_t y = 0; y < 1000; y++) {
            for(uint16_t x = 0; x <1000; x++) {
                double value = Pyro::noise(x / 30.0, y / 30.0);
                REQUIRE(value >= 0.0);
                REQUIRE(value <= 1.0);
            }
        }
    }
}