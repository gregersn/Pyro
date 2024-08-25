#include <catch2/catch_all.hpp>
#include "test-settings.h"
#include "pyro/graphics.h"

TEST_CASE("Write text")
{
    std::filesystem::path filename = "";
    SECTION("Simple strings")
    {
        Pyro::Graphics *p = Pyro::creategraphics(100, 100, testmode);
        Pyro::Font *fnt = Pyro::create_font("./fonts/Orkney Bold.otf", 12);

        filename = "typography_simple_string.png";

        p->background(192);
        p->textfont(fnt);
        p->textsize(32);
        p->text("word", 10, 30);
        p->fill(0, 102, 153);
        p->text("word", 10, 60);
        p->fill(0, 102, 153, 51);
        p->text("word", 10, 90);

        p->save(current_folder / filename);
        delete p;
        CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename, 4.02942e-05));
    }
}
