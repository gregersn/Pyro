
#include <catch2/catch_all.hpp>

#include "pyro/pyro.h"
#include "pyro/runner.h"
#include <SDL3/SDL_events.h>

using namespace Pyro;

int test_setup_result{0};

void test_setup_setup()
{
    test_setup_result = 1;
}

void test_setup_draw()
{
    quit();
}

TEST_CASE("Test the realtime runner", "[runner]")
{
    run(test_setup_setup, test_setup_draw, true);
    REQUIRE(test_setup_result == 1);
}
