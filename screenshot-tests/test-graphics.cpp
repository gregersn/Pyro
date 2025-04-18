#include <catch2/catch_all.hpp>
#include "pyro/graphics.h"
#include "test-settings.h"

#include <filesystem>

TEST_CASE("Draw multiple times")
{
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);

    Pyro::Graphics *a = Pyro::creategraphics(100, 40, mode);
    Pyro::Graphics *b = Pyro::creategraphics(100, 40, mode);

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

TEST_CASE("Test triangle")
{
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
    std::filesystem::path filename = "graphics_triangle.png";

    Pyro::Graphics *triangle = Pyro::creategraphics(256, 256, mode);
    triangle->begindraw();
    triangle->fill(1.0f, 0.0f, 1.0f, 1.0f);
    triangle->stroke(0.0f, 1.0f, 0.0f, 1.0f);
    triangle->triangle(128, 5, 251, 251, 5, 251);
    triangle->enddraw();
    triangle->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
    delete triangle;
    CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
}

TEST_CASE("Test smoothing")
{
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
    std::filesystem::path filename = "graphics_smooth.png";

    Pyro::Graphics *smoothing = Pyro::creategraphics(9 * 50, 50, mode);
    smoothing->begindraw();
    smoothing->ellipsemode(Pyro::CENTER);
    for (unsigned int i = 0; i < 9; i++)
    {
        smoothing->smooth(i);
        smoothing->ellipse(25, 25, 25, 25);
        smoothing->translate(50, 0);
    }
    smoothing->enddraw();
    smoothing->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
    delete smoothing;
    CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
}

TEST_CASE("Test blend modes")
{

    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
    std::filesystem::path filename = "graphics_blend.png";
    Pyro::Graphics *blending = Pyro::creategraphics(300, 300, mode);

    blending->nostroke();

    blending->blendmode(Pyro::BLEND);
    blending->fill(1.0f, 0.0f, 0.0f, .5f);
    blending->rect(0, 0, 125, 125);

    blending->blendmode(Pyro::REPLACE);
    blending->fill(0.0f, 1.0f, 0.0f, .5f);
    blending->rect(100, 0, 125, 125);

    blending->blendmode(Pyro::ADD);
    blending->fill(0.0f, 0.0f, 1.0f, .5f);
    blending->rect(200, 0, 125, 125);

    blending->blendmode(Pyro::LIGHTEST);
    blending->fill(1.0f, 0.5f, 0.0f, .5f);
    blending->rect(0, 100, 125, 125);

    blending->blendmode(Pyro::DARKEST);
    blending->fill(0.0f, 1.0f, 0.5f, .5f);
    blending->rect(100, 100, 125, 125);

    blending->blendmode(Pyro::DIFFERENCE);
    blending->fill(0.5f, 0.0f, 1.0f, .5f);
    blending->rect(200, 100, 125, 125);

    blending->blendmode(Pyro::EXCLUSION);
    blending->fill(0.0f, 0.5f, 1.0f, .5f);
    blending->rect(0, 200, 125, 125);

    blending->blendmode(Pyro::MULTIPLY);
    blending->fill(1.0f, 0.0f, 0.5f, .5f);
    blending->rect(100, 200, 125, 125);

    blending->blendmode(Pyro::SCREEN);
    blending->fill(0.5f, 1.0f, 0.0f, .5f);
    blending->rect(200, 200, 125, 125);

    blending->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
    delete blending;
    CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename, 0.00105));
}
