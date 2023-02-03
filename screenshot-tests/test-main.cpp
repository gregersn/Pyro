#include <catch2/catch_all.hpp>
#include <catch2/catch_session.hpp>
#include <cstdlib>
#include "test-settings.h"

#include "pyro/pyro.h"

TEST_CASE("Create test image")
{
    std::string filename = "test_image.png";
    Pyro::Graphics *p = Pyro::create(128, 128, testmode);

    p->nostroke();

    p->fill(1.0f, 0.0f, 0.0f, 1.0f);
    p->rect(0, 0, p->width() / 2.0f, p->height() / 2.0f);

    p->fill(0.0f, 1.0f, 0.0f, 1.0f);
    p->rect(p->width() / 2.0f, 0, p->width() / 2.0f, p->height() / 2.0f);

    p->fill(0.0f, 0.0f, 1.0f, 1.0f);
    p->rect(0, p->height() / 2.0f, p->width() / 2.0f, p->height() / 2.0f);

    p->fill(1.0f, 1.0f, 0.0f, 1.0f);
    p->rect(p->width() / 2.0f, p->height() / 2.0f, p->width() / 2.0f, p->height() / 2.0f);

    p->nofill();
    p->stroke(0.0f, 1.0f);
    p->strokeweight(5.0f);

    p->line(p->width() / 2.0f, 0, p->width() / 2.0f, p->height());
    p->line(p->width() / 2.0f, 0, 0, p->height() / 2.0f);
    p->line(p->width() / 2.0f, 0, p->width(), p->height() / 2.0f);

    p->save(current_folder + filename);
    delete p;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}

int main(int argc, char *argv[])
{
    const int direrr = system("mkdir -p ./screenshots/current");
    if (direrr == -1)
    {
        exit(1);
    }

    int result = Catch::Session().run(argc, argv);
    return result;
}
