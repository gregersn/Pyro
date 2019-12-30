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

TEST_CASE("Stroke can be set", "[graphics]") {
    Pyro::Graphics *pg = Pyro::Graphics::create(10, 1);

    SECTION("Set no stroke") {
        pg->nostroke();
        pg->nosmooth();
        pg->nofill();

        uint32_t p = pg->get(0, 0);
        pg->line(0, 0, 10, 0);

        uint32_t q = pg->get(0, 0);

        REQUIRE(p == q);
    }

    SECTION("Set red stroke") {
        pg->nofill();
        pg->nosmooth();
        pg->stroke(1.0f, 0.0f, 0.0f, 1.0f);

        uint32_t p = pg->get(0, 0);
        pg->line(0, 0, 10, 0);

        uint32_t q = pg->get(0, 0);

        REQUIRE(p == 0x00000000);
        REQUIRE(q == 0xffff0000);

    }
}

TEST_CASE("Shapes can be drawn", "[graphics]") {
    Pyro::Graphics *pg = Pyro::Graphics::create(8, 8);

    SECTION("Draw a filled square") {
        pg->nosmooth();
        pg->nostroke();
        pg->fill(0.0f, 1.0f, 0.0f, 1.0f);

        pg->beginshape();
        pg->vertex(3.0f, 3.0f);
        pg->vertex(6.0f, 3.0f);
        pg->vertex(6.0f, 6.0f);
        pg->vertex(3.0f, 6.0f);
        pg->vertex(3.0f, 3.0f);
        pg->endshape();

        uint32_t p = pg->get(0, 0);
        REQUIRE(p == 0x00000000);

        uint32_t q = pg->get(4, 4);
        REQUIRE(q == 0xff00ff00);

    }

    SECTION("Draw with curvevertex") {
        pg->nostroke();
        pg->fill(0.0f, 1.0f, 0.0f, 1.0f);

        pg->beginshape();
        pg->vertex(0, 0);
        pg->curvevertex(2, 6);
        pg->curvevertex(6, 3);
        pg->vertex(0, 0);

        pg->endshape();
    }
}

TEST_CASE("Select different color modes") {
    Pyro::Graphics *pg = Pyro::Graphics::create(3, 3);

    SECTION("Use HSB") {
        pg->nosmooth();
        pg->nostroke();
        pg->colormode(Pyro::ColorMode::HSB);
        pg->fill(.5f, 1.0f, .5f);
        pg->rect(0, 0, 2, 2);

        uint32_t p = pg->get(1, 1);
        REQUIRE(p == 0xff008080);
    }
}

TEST_CASE("Save and load images") {
    Pyro::Graphics *pg = Pyro::Graphics::create(4, 4);

    SECTION("Create, save and load") {
        pg->background(1.0f, 0.0f, 1.0f, 1.0f);
        pg->save("__testimage__.png");

        Pyro::Image *img = pg->load("__testimage__.png");

        uint32_t p = pg->get(1, 1);
        uint32_t q = img->get(1, 1);

        REQUIRE(p == q);
        REQUIRE(p == 0xffff00ff);
    }
}