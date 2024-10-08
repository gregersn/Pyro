#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include <filesystem>

TEST_CASE("Draw multiple times")
{
    Pyro::Graphics *a = Pyro::creategraphics(100, 40);
    Pyro::Graphics *b = Pyro::creategraphics(100, 40);

    a->begindraw();
    a->background(0.0f, 1.0f);
    a->fill(1.0f, 0.0f, 0.0f, 1.0f);
    a->rect(10, 10, 60, 20);
    a->enddraw();

    a->save("multidraw_a_1.png");

    b->begindraw();
    b->image(a, 0, 0);
    b->enddraw();

    b->save("multidraw_b_1.png");

    CHECK_THAT("multidraw_a_1.png", LooksLike("multidraw_b_1.png"));

    a->begindraw();
    a->background(0.0f, 1.0f);
    a->fill(0.0f, 1.0f, 0.0f, 1.0f);
    a->rect(5, 5, 60, 20);
    a->enddraw();

    a->save("multidraw_a_2.png");

    b->begindraw();
    b->image(a, 0, 0);
    b->enddraw();

    b->save("multidraw_b_2.png");

    CHECK_THAT("multidraw_a_2.png", LooksLike("multidraw_b_2.png"));

    std::remove("multidraw_a_1.png");
    std::remove("multidraw_a_2.png");
    std::remove("multidraw_b_1.png");
    std::remove("multidraw_b_2.png");
}
