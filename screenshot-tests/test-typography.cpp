#include <catch2/catch.hpp>
#include "test-settings.h"
#include "pyro/pyrographics.h"

TEST_CASE("Write text") {
    std::string filename = "";
    SECTION("Simple strings") {
        Pyro::Graphics *p = Pyro::Graphics::create(100, 100, testmode);
        filename ="typography_simple_string.png";
        
        p->background(192);
        p->textsize(32);
        p->text("word", 10, 30); 
        p->fill(0, 102, 153);
        p->text("word", 10, 60);
        p->fill(0, 102, 153, 51);
        p->text("word", 10, 90);

        p->save(current_folder + filename);
        delete p;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename, 4.02942e-05));
    }
}