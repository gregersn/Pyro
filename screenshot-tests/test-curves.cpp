#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"
#include "pyro/shape.h"

using namespace Pyro;

TEST_CASE("Test curvepoint", "[shapes]")
{
    std::filesystem::path filename = "curve_curvepoints.png";

    size(400, 400);

    nofill();
    curve(20, 104, 20, 104, 292, 96, 292, 244);
    curve(20, 104, 292, 96, 292, 244, 60, 260);

    fill(255);
    ellipsemode(CENTER);
    int steps = 6;
    for (int i = 0; i <= steps; i++)
    {
        float t = i / float(steps);
        float x = curvepoint(20.f, 20.f, 292.f, 292.f, t);
        float y = curvepoint(104.f, 104.f, 96.f, 244.f, t);
        ellipse(x, y, 10, 10);
        x = curvepoint(20, 292, 292, 60, t);
        y = curvepoint(104, 96, 244, 260, t);
        ellipse(x, y, 10, 10);
    }

    save(current_folder / filename);

    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
}

TEST_CASE("Test bezierpoint", "[shapes]")
{
    std::filesystem::path filename = "curve_bezierpoint.png";

    size(400, 400);
    nofill();
    bezier(340, 80, 40, 40, 360, 360, 60, 320);
    fill(255);
    ellipsemode(CENTER);
    int steps = 10;
    for (int i = 0; i <= steps; i++)
    {
        float t = i / float(steps);
        float x = bezierpoint(340, 40, 360, 60, t);
        float y = bezierpoint(80, 40, 360, 320, t);
        ellipse(x, y, 10, 10);
    }
    save(current_folder / filename);

    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
}

TEST_CASE("Test bspline", "[shapes]")
{
    std::filesystem::path filename = "curve_bspline.png";

    std::vector points = std::vector{
        Vector(-100, .0),
        Vector(-50, 50),
        Vector(50, -50),
        Vector(100.0, 0),
    };

    std::vector knots = std::vector{0, 0, 0, 1, 2, 2, 2};

    size(500, 500);

    nofill();

    pushmatrix();
    translate(150, 100);

    stroke(0.0, 0.0, 0.0, 0.5f);

    int degree = 2;

    // Draw the curve
    bsplinecurve(points, degree, {0, 1, 2, 3, 4, 5, 6});
    int resolution = 20;
    double increment = (1.0f) / (double)(resolution - 1);
    float t = 0.0f;

    stroke(0.0, 0.0, 1.0, 0.5f);
    // Draw the curve as ellipsies
    for (int i = 0; i < resolution; i++)
    {

        Vector p = bsplinepoint(points, t, 2);
        ellipse(p.x, p.y, 5, 5);
        t += increment;
    }

    // Draw the linear version from the points.
    nofill();
    stroke(1.0f, 0.0f, 1.0, .5f);
    beginshape();
    for (unsigned int i = 0; i < points.size(); i++)
    {
        vertex(points[i]);
    }
    endshape(OPEN);

    popmatrix();

    pushmatrix();
    translate(150, 300);

    stroke(0.0, 0.0, 0.0, 0.5f);
    bsplinecurve(points, degree, knots);

    t = 0.0f;
    stroke(0.0, 0.0, 1.0, 0.5f);

    // Draw the curve as ellipsies
    for (int i = 0; i < resolution; i++)
    {
        Vector p = bsplinepoint(points, t, 2, false, knots);
        ellipse(p.x, p.y, 5, 5);
        t += increment;
    }

    // Draw the linear version from the points.
    nofill();
    stroke(1.0f, 0.0f, 1.0, .5f);
    beginshape();
    for (unsigned int i = 0; i < points.size(); i++)
    {
        vertex(points[i]);
    }
    endshape(OPEN);
    popmatrix();

    // Closed curve

    std::vector closed_points = std::vector{
        Vector(-100, .0),
        Vector(-50, 50),
        Vector(50, -50),
        Vector(100.0, 0),

        // Repeat first degree + 1 points
        Vector(-100, .0),
        Vector(-50, 50),
        Vector(50, -50),

    };

    std::vector closed_knots = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    curvedetail(200);

    pushmatrix();
    translate(375, 100);

    stroke(0.0, 0.0, 0.0, 0.5f);
    bsplinecurve(closed_points, degree, closed_knots);

    t = 0.0f;
    stroke(0.0, 0.0, 1.0, 0.5f);

    // Draw the curve as ellipsies
    for (int i = 0; i < resolution; i++)
    {
        Vector p = bsplinepoint(closed_points, t, 2, false, closed_knots);
        ellipse(p.x, p.y, 5, 5);
        t += increment;
    }

    // Draw the linear version from the points.
    nofill();
    stroke(1.0f, 0.0f, 1.0, .5f);
    beginshape();
    for (unsigned int i = 0; i < closed_points.size(); i++)
    {
        vertex(closed_points[i]);
    }
    endshape(OPEN);
    popmatrix();

    save(current_folder / filename);

    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
}
