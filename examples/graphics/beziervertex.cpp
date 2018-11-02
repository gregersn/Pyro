#include <pyro.h>

using namespace Pyro;

int main(int argc, char **argv) {
    size(100, 200);
    background(192);

    nofill();
    beginshape();
    vertex(30, 20);
    beziervertex(80, 0, 80, 75, 30, 75);
    endshape();

    translate(0, 100);

    fill(255);
    beginshape();
    vertex(30, 20);
    beziervertex(80, 0, 80, 75, 30, 75);
    beziervertex(50, 80, 60, 25, 30, 20);
    endshape();

    save("output/beziervertex.png");
    return 0;
}