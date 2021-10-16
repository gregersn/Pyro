#include "pyro/pyro.h"

using namespace Pyro;

float a;

void setup()
{
    size(640, 360);
    stroke(255);
    a = height / 2;
}

void draw()
{
    background(51);
    line(0, a, width, a);
    a = a - 0.5;
    if (a < 0)
    {
        a = height;
    }
}

int main(int argc, char **argv)
{
    run(setup, draw);
    return 0;
}
