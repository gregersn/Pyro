#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include <filesystem>

TEST_CASE("Push and pop style")
{
    std::filesystem::path filename = "push_pop_style.png";

    Pyro::Graphics *p = Pyro::creategraphics(400, 400);

    p->curvedetail(132);

    p->ellipse(0, 200, 132, 132); // Left circle

    p->pushstyle(); // Start a new style
    p->strokeweight(40);
    p->fill(204, 153, 0);

    p->ellipse(200, 200, 132, 132); // Middle circle
    p->popstyle();                  // Restore original style

    p->ellipse(400, 200, 132, 132); // Right circle

    p->save(current_folder / filename);
    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
}
