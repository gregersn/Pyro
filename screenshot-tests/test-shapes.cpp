#include <catch2/catch.hpp>
#include "test-settings.h"
#include "pyro/pyrographics.h"

TEST_CASE("Test Rect") {
    std::string filename = "";

    SECTION("Corner drawing") {
        Pyro::Graphics *p = Pyro::Graphics::create(250, 250);
        filename ="shape_rect_corner.png";
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

        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));

    }

    SECTION("Center drawing") {
        Pyro::Graphics *p = Pyro::Graphics::create(250, 250);
        std::string filename ="shape_rect_center.png";
        p->rectmode(Pyro::RectMode::CENTER);
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

        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }


}
