#include <pyro.h>
#include <math.h>

using namespace Pyro;

void polygon(float x, float y, float radius, int npoints) {
  float angle = TWO_PI / npoints;
  beginshape();
  for (float a = 0; a < TWO_PI; a += angle) {
    float sx = x + cos(a) * radius;
    float sy = y + sin(a) * radius;
    vertex(sx, sy);
  }
  endshape(CLOSE);
}

int main(int argc, char **argv) {
    int angles[] = { 30, 10, 45, 35, 60, 38, 75, 67 };

    size(640, 360);

    background(102);

    pushmatrix();
    translate(width * 0.2, height * 0.5);
    // rotate(frameCount / 200.0);
    polygon(0, 0, 82, 3);  // Triangle
    popmatrix();
    
    pushmatrix();
    translate(width * 0.5, height * 0.5);
    // rotate(frameCount / 50.0);
    polygon(0, 0, 80, 20);  // Icosahedron
    popmatrix();

    pushmatrix();
    translate(width * 0.8, height * 0.5);
    // rotate(frameCount / -100.0);
    polygon(0, 0, 70, 7);  // Heptagon
    popmatrix();

    save("output/regular_polygons.png");

}
