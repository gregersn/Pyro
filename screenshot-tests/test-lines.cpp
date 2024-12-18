#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/graphics.h"

TEST_CASE("Draw line", "[strokes]")
{
    Pyro::Graphics *p = Pyro::creategraphics(100, 100, testmode);

    SECTION("Stroke weight")
    {
        std::filesystem::path filename = "stroke_weight.png";
        p->background(192);

        p->strokeweight(.5); //  Thin
        p->line(20, 15, 80, 15);
        p->strokeweight(1); // Default
        p->line(20, 25, 80, 25);
        p->strokeweight(4); // Thicker
        p->line(20, 45, 80, 45);
        p->strokeweight(10); // Beastly
        p->line(20, 75, 80, 75);

        p->save(current_folder / filename);
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }

    SECTION("Stroke cap")
    {
        std::filesystem::path filename = "stroke_cap.png";
        p->background(192);

        p->strokeweight(12.0);
        p->strokecap(Pyro::ROUND);
        p->line(20, 30, 80, 30);
        p->strokecap(Pyro::SQUARE);
        p->line(20, 50, 80, 50);
        p->strokecap(Pyro::PROJECT);
        p->line(20, 70, 80, 70);

        p->save(current_folder / filename);
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }

    SECTION("Stroke join")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 300, testmode);

        std::filesystem::path filename = "stroke_join.png";
        p->background(192);

        p->nofill();
        p->strokeweight(10.0);
        p->strokejoin(Pyro::MITER);
        p->beginshape();
        p->vertex(35, 20);
        p->vertex(65, 50);
        p->vertex(35, 80);
        p->endshape();

        p->translate(0, 100);
        p->nofill();
        p->strokeweight(10.0);
        p->strokejoin(Pyro::BEVEL);
        p->beginshape();
        p->vertex(35, 20);
        p->vertex(65, 50);
        p->vertex(35, 80);
        p->endshape();

        p->translate(0, 100);
        p->nofill();
        p->strokeweight(10.0);
        p->strokejoin(Pyro::ROUND);
        p->beginshape();
        p->vertex(35, 20);
        p->vertex(65, 50);
        p->vertex(35, 80);
        p->endshape();

        p->save(current_folder / filename);
        delete p;

        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }

    delete p;
}

TEST_CASE("Test curve", "[shapes]")
{
    SECTION("Draw with curvevertex()")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 100, testmode);
        std::filesystem::path filename = "shape_curve_vertex.png";
        p->background(192);
        p->nofill();
        p->beginshape();
        p->curvevertex(84, 91);
        p->curvevertex(84, 91);
        p->curvevertex(68, 19);
        p->curvevertex(21, 17);
        p->curvevertex(32, 100);
        p->curvevertex(32, 100);
        p->endshape();
        p->save(current_folder / filename);
        delete p;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }

    SECTION("Draw with curve()")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 100, testmode);
        std::filesystem::path filename = "shape_curve.png";
        p->background(192);
        p->nofill();
        p->stroke(255, 102, 0);
        p->curve(5, 26, 5, 26, 73, 24, 73, 61);
        p->stroke(0);
        p->curve(5, 26, 73, 24, 73, 61, 15, 65);
        p->stroke(255, 102, 0);
        p->curve(73, 24, 73, 61, 15, 65, 15, 65);

        p->save(current_folder / filename);
        delete p;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
}

TEST_CASE("Bezier curve", "[shapes]")
{
    std::filesystem::path filename = "";
    SECTION("Draw with beziervertex()")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 100, testmode);
        filename = "shape_curve_bezier_vertex.png";
        p->background(192);
        p->nofill();
        p->beginshape();
        p->vertex(30, 20);
        p->beziervertex(80, 0, 80, 75, 30, 75);
        p->endshape();
        p->save(current_folder / filename);
        delete p;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }

    SECTION("Draw with bezier()")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 200, testmode);
        std::filesystem::path filename = "shape_curve_bezier.png";
        p->background(192);
        p->nofill();

        p->stroke(255, 102, 0);
        p->line(85, 20, 10, 10);
        p->line(90, 90, 15, 80);
        p->stroke(0, 0, 0);
        p->bezier(85, 20, 10, 10, 90, 90, 15, 80);

        p->translate(0, 100);

        p->stroke(255, 102, 0);
        p->line(30, 20, 80, 5);
        p->line(80, 75, 30, 75);
        p->stroke(0, 0, 0);
        p->bezier(30, 20, 80, 5, 80, 75, 30, 75);

        p->save(current_folder / filename);
        delete p;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
    }
}
