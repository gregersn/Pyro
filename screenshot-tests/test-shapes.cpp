#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/graphics.h"

TEST_CASE("Test Rect", "[shapes]")
{
    std::filesystem::path filename = "";
    Pyro::Graphics *p{nullptr};
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);

    SECTION("Corner drawing")
    {
        p = Pyro::creategraphics(250, 250, mode);
        filename = "shape_rect_corner.png";
        p->nofill();
        p->stroke(0.0f, 1.0f);
        p->rect(10, 10, 100, 100);

        p->nostroke();
        p->fill(0.0f, 1.0f);
        p->rect(120, 10, 100, 100);

        p->stroke(0.0f, 1.0f);
        p->fill(0.0f, 1.0f, 0.0f, 1.0f);
        p->rect(10, 120, 100, 100);

        p->nostroke();
        p->nofill();
        p->rect(120, 120, 100, 100);
    }

    SECTION("Center drawing")
    {
        p = Pyro::creategraphics(250, 250, mode);
        filename = "shape_rect_center.png";
        p->rectmode(Pyro::CENTER);
        p->nofill();
        p->stroke(0.0f, 1.0f);
        p->rect(60, 60, 100, 100);

        p->nostroke();
        p->fill(0.0f, 1.0f);
        p->rect(170, 60, 100, 100);

        p->stroke(0.0f, 1.0f);
        p->fill(0.0f, 0.0f, 1.0f, 1.0f);
        p->rect(60, 170, 100, 100);

        p->nostroke();
        p->nofill();
        p->rect(170, 170, 100, 100);
    }
    p->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
    delete p;
    CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
}

TEST_CASE("Draw shapes", "[shapes]")
{
    auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
    Pyro::Graphics *p{nullptr};
    std::filesystem::path filename = "";

    SECTION("Draw triangle")
    {
        p = Pyro::creategraphics(128, 128, mode);
        filename = "shape_triangle.png";
        p->background(192);
        p->nostroke();
        p->fill(0, 128, 255);
        p->triangle(p->width() / 2, 0, 0, p->height() - 1, p->width() - 1, p->height() - 1);
    }

    SECTION("Draw circle")
    {
        p = Pyro::creategraphics(128, 128, mode);
        filename = "shape_circle.png";
        p->background(192);
        p->nostroke();
        p->fill(64, 64, 255);
        p->curvedetail(64);
        p->ellipse(p->width() / 2, p->height() / 2, 60.f);
    }

    SECTION("Draw ellipse")
    {
        p = Pyro::creategraphics(128, 128, mode);
        filename = "shape_ellipse.png";
        p->background(192);
        p->nostroke();
        p->fill(255, 64, 128);
        p->curvedetail(64);
        p->ellipse(p->width() / 2, p->height() / 2, 60, 120);
    }

    p->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
    delete p;
    CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
}

TEST_CASE("Variable side count ellipses", "[shapes]")
{
    SECTION("Draw ellipses with n siders")
    {
        auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
        Pyro::Graphics *p = Pyro::creategraphics(512, 512, mode);
        std::filesystem::path filename = "shape_n_ellipses.png";
        p->background(192);
        p->fill(255, 64, 128);
        p->ellipsemode(Pyro::CENTER);

        for (uint32_t y = 0; y < 4; y++)
        {
            p->pushmatrix();
            for (uint32_t x = 0; x < 4; x++)
            {
                p->curvedetail((y * 4) + x + 3);
                p->ellipse(512 / 8, 512 / 8, 500 / 4, 500 / 4);
                p->translate(512 / 4, 0);
            }
            p->popmatrix();
            p->translate(0, 512 / 4);
        }
        p->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
        delete p;
        CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
    }
}

TEST_CASE("Complex shapes", "[shapes]")
{
    SECTION("Draw shape with hole")
    {
        auto mode = GENERATE(Pyro::GraphicsMode::CAIRO, Pyro::GraphicsMode::SDL);
        Pyro::Graphics *p = Pyro::creategraphics(100, 100, mode);
        std::filesystem::path filename = "shape_multi_contour.png";
        p->translate(50, 50);
        p->stroke(255, 0, 0);
        p->beginshape();
        // Exterior part of shape, clockwise winding
        p->vertex(-40, -40);
        p->vertex(40, -40);
        p->vertex(40, 40);
        p->vertex(-40, 40);
        // Interior part of shape, counter-clockwise winding
        p->begincontour();
        p->vertex(-20, -20);
        p->vertex(-20, 20);
        p->vertex(20, 20);
        p->vertex(20, -20);
        p->endcontour();
        p->endshape(Pyro::CLOSE);

        p->save(current_folder / Pyro::GraphicsModeName[mode] / filename);
        delete p;
        CHECK_THAT(current_folder / Pyro::GraphicsModeName[mode] / filename, LooksLike(actual_folder / Pyro::GraphicsModeName[mode] / filename));
    }
}
