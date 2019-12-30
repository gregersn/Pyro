#include <pyro/pyro.h>

using namespace Pyro;

int main(int argc, char **argv) {
    size(240, 240);

    arc(50, 55, 50, 50, 0, Pyro::HALF_PI);
    nofill();
    arc(50, 55, 60, 60, Pyro::HALF_PI, Pyro::PI);
    arc(50, 55, 70, 70, Pyro::PI, Pyro::PI+Pyro::QUARTER_PI);
    arc(50, 55, 80, 80, Pyro::PI+Pyro::QUARTER_PI, Pyro::TWO_PI);

    fill(255);
    
    translate(100, 0);
    arc(50, 50, 80, 80, 0, Pyro::PI + Pyro::QUARTER_PI, Pyro::OPEN);

    translate(-100, 100);
    arc(50, 50, 80, 80, 0, Pyro::PI + Pyro::QUARTER_PI, Pyro::CHORD);

    translate(100, 0);
    arc(50, 50, 80, 80, 0, Pyro::PI + Pyro::QUARTER_PI, Pyro::PIE);

    save("output/arc.png");

}
