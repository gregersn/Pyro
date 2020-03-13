#include <catch2/catch.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Write text") {
    std::string filename = "";
    SECTION("Simple strings") {
        Pyro::Graphics *p = Pyro::Graphics::create(100, 100);
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
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
    }    
}

TEST_CASE("Write text with loaded font") {
    std::string filename = "";
    SECTION("Write simple string") {
        Pyro::Graphics *p = Pyro::Graphics::create(100, 100);
        std::string fontfile = actual_folder + "Zantroke-gmR5.otf";
        INFO("Loading font: " << fontfile << "\n");
        Pyro::Font *font = Pyro::loadfont(fontfile);
        REQUIRE(font != nullptr);
        /*filename ="typography_simple_string_with_loaded_font.png";
        
        p->background(192);
        p->textsize(32);
        p->textfont(font);
        p->text("word", 10, 30); 
        p->fill(0, 102, 153);
        p->text("word", 10, 60);
        p->fill(0, 102, 153, 51);
        p->text("word", 10, 90);

        p->save(current_folder + filename);
        delete p;
        delete font;
        CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));*/
    }    
}