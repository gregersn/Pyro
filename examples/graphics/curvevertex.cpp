#include "pyro/pyro.h"

using namespace Pyro;

int main(int argc, char **argv)
{
    size(100, 200);
    background(192);

    nofill();
    beginshape();
    curvevertex(84, 91);
    curvevertex(84, 91);
    curvevertex(68, 19);
    curvevertex(21, 17);
    curvevertex(32, 100);
    curvevertex(32, 100);
    endshape();

    save("output/curvevertex.png");
    return 0;
}