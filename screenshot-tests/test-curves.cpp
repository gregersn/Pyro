#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

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

TEST_CASE("Test beierpoint", "[shapes]")
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
