#include <pyro.h>

using namespace Pyro;

int main(int argc, char **argv) {
    Shape s;

    size(100, 100);

    // Make a shape
    s = createshape();
    s.begin();
    //s.nostroke();


    // Exterior part of shape
    s.vertex(-50,-50);
    s.vertex(50,-50);
    s.vertex(50,50);
    s.vertex(-50,50);

    // Interior part of shape
    s.begincontour();
    s.vertex(-20,-20);
    s.vertex(-20,20);
    s.vertex(20,20);
    s.vertex(20,-20);
    s.endcontour();

    // Finish off shape
    s.end();

    background(52);
    translate(width/2, height/2);
    s.rotate(0.1);
    shape(s);

    save("output/contour.png");
}