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

        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }


}

TEST_CASE("Test curve") {
    std::string filename = "";

    SECTION("Draw with curvevertex()") {
        Pyro::Graphics *p = Pyro::Graphics::create(100, 100);
        std::string filename = "shape_curve_vertex.png";
        p->background(192);
        p->nofill();
        p->beginshape();
        p->curvevertex(84,  91);
        p->curvevertex(84,  91);
        p->curvevertex(68,  19);
        p->curvevertex(21,  17);
        p->curvevertex(32, 100);
        p->curvevertex(32, 100);
        p->endshape();
        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));

    }

    SECTION("Draw with curve()") {
        Pyro::Graphics *p = Pyro::Graphics::create(100, 100);
        std::string filename = "shape_curve.png";
        p->background(192);
        p->nofill();
        p->stroke(255, 102, 0);
        p->curve(5, 26, 5, 26, 73, 24, 73, 61);
        p->stroke(0); 
        p->curve(5, 26, 73, 24, 73, 61, 15, 65); 
        p->stroke(255, 102, 0);
        p->curve(73, 24, 73, 61, 15, 65, 15, 65);

        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }

}