#include "pyro/pyro.h"

using namespace Pyro;

int main(int argc, char **argv)
{
    size(640, 360);

    background(0);
    nostroke();
    fill(204);
    triangle(18, 18, 18, 360, 81, 360);

    fill(102);
    rect(81, 81, 63, 63);

    fill(204);
    quad(189, 18, 216, 18, 216, 360, 144, 360);

    fill(255);
    ellipse(252, 144, 72, 72);

    fill(204);
    triangle(288, 18, 351, 360, 288, 360);

    fill(255);
    arc(479, 300, 280, 280, M_PI, M_PI * 2);

    save("output/graphics_test.png");

    return 0;
}
