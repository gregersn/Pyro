#include <catch2/catch.hpp>
#include "test-settings.h"
#include "pyro/pyro.h"

TEST_CASE("Place image full screen") {
    std::string filename = "image_place_default.png";

    Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");
    if(img == nullptr) {
        FAIL_CHECK("Test image does not exist");
        return;
    }

    Pyro::Graphics *p = Pyro::Graphics::create(img->width(), img->height());
    p->image(img, 0, 0);

    p->save(current_folder + filename);

    delete img;
    delete p;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}

TEST_CASE("Place image 50% at center rotated 45 degrees") {
    std::string filename = "image_place_scaled_rotated.png";

    Pyro::Image *img = Pyro::Image::load(actual_folder + "test_image.png");
    if(img == nullptr) {
        FAIL_CHECK("Test image does not exist");
        return;
    }

    Pyro::Graphics *p = Pyro::Graphics::create(img->width(), img->height());
    p->imagemode(Pyro::CENTER);
    p->translate(p->width() / 2.0f, p->height() / 2.0f);
    p->rotate(Pyro::QUARTER_PI);
    p->scale(0.5, 0.5);
    p->image(img, 0, 0);

    p->save(current_folder + filename);
    
    delete img;
    delete p;

    CHECK_THAT(current_folder + filename, LooksLike(actual_folder + filename));
}