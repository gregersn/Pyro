#include "catch.hpp"

#include <pyro.h>

TEST_CASE("JSON files", "[json]") {
    SECTION("can be read") {
        auto data = Pyro::load_json("test.json");
        REQUIRE(data["foo"] == "bar");
        REQUIRE(data["pi"] == 3);
    }
}
