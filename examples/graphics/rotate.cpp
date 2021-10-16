#include "pyro/pyro.h"

using namespace Pyro;

int main(int argc, char **argv)
{
    size(640, 640);

    translate(width / 2.0, height / 2.0);
    rotate(PI / 3.0);
    rect(-26, -26, 52, 52);
    save("output/rotate.png");
}