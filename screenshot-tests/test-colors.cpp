#include <catch2/catch_all.hpp>
#include "test-settings.h"

#include <filesystem>

SCENARIO("A color can be specified with various types")
{
    Pyro::Graphics *p{nullptr};
    std::filesystem::path filename = "";

    SECTION("Fill can be set with ints")
    {
        p = Pyro::creategraphics(1024, 512, testmode);
        filename = "fill_with_ints.png";
        p->nostroke();
        for (int i = 0; i < 256; i++)
        {
            p->fill(i);
            p->rect(i * 4, 0, 4, 128);

            p->fill(i, 0, 0);
            p->rect(i * 4, 128, 4, 128);

            p->fill(0, i, 0);
            p->rect(i * 4, 256, 4, 128);

            p->fill(0, 0, i);
            p->rect(i * 4, 384, 4, 128);
        }
    }
    SECTION("Fill can be set with floas")
    {
        p = Pyro::creategraphics(1024, 512, testmode);
        filename = "fill_with_floats.png";
        p->nostroke();
        for (int i = 0; i < 256; i++)
        {
            p->fill(i / 255.0f);
            p->rect(i * 4, 0, 4, 128);

            p->fill(i / 255.0f, 0.f, 0.f);
            p->rect(i * 4, 128, 4, 128);

            p->fill(0.f, i / 255.0f, 0.f);
            p->rect(i * 4, 256, 4, 128);

            p->fill(0.f, 0.f, i / 255.0f);
            p->rect(i * 4, 384, 4, 128);
        }
    }
    p->save(current_folder / filename);
    delete p;
    CHECK_THAT(current_folder / filename, LooksLike(actual_folder / filename));
}
