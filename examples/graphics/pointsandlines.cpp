#include "pyro/pyro.h"

using namespace Pyro;

int main(int argc, char **argv)
{
    int d = 70;
    int p1 = d;
    int p2 = p1 + d;
    int p3 = p2 + d;
    int p4 = p3 + d;

    size(640, 360);

    nosmooth();
    background(0);

    translate(140, 0);

    // Draw gray box
    stroke(153);
    line(p3, p3, p2, p3);
    line(p2, p3, p2, p2);
    line(p2, p2, p3, p2);
    line(p3, p2, p3, p3);

    // Draw white points
    stroke(255);
    point(p1, p1);
    point(p1, p3);
    point(p2, p4);
    point(p3, p1);
    point(p4, p2);
    point(p4, p4);

    save("output/points_and_lines.png");

    return 0;
}
