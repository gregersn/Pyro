#include <catch2/catch.hpp>

#include <pyrographics.h>

TEST_CASE("Graphics objects can be initialized", "[graphics]") {
    SECTION("Create a graphics object") {
        Pyro::Graphics *graphics = Pyro::Graphics::create(400, 300);

        REQUIRE(graphics->width() == 400);
        REQUIRE(graphics->height() == 300);
    }
}

TEST_CASE("Graphics can have backgrounds", "[graphics]") {
    Pyro::Graphics *pg = Pyro::Graphics::create(4, 4);

    SECTION("Set a red background") {
        pg->background(1.0f, 0.0f, 0.0f);
        uint32_t p = pg->get(0, 0);

        REQUIRE(p == 0xffff0000);

    }

    SECTION("Set a green background") {
        pg->background(0.0f, 1.0f, 0.0f);
        uint32_t p = pg->get(0, 0);

        REQUIRE(p == 0xff00ff00);
    }

    SECTION("Set a blue background") {
        pg->background(0.0f, 0.0f, 1.0f);
        uint32_t p = pg->get(0, 0);

        REQUIRE(p == 0xff0000ff);
    }
}

TEST_CASE("Fill can be set", "[graphics]") {
    Pyro::Graphics *pg = Pyro::Graphics::create(8, 8);

    SECTION("Set a red fill") {
        pg->fill(1.0f, 0.0f, 0.0f);
        pg->nostroke();
        pg->rect(2, 2, 4, 4);

        uint32_t p = pg->get(0, 0);
        uint32_t q = pg->get(4, 4);

        REQUIRE(p == 0x00000000);
        REQUIRE(q == 0xffff0000);
    }
}