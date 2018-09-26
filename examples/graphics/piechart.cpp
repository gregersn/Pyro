#include <pyro.h>

using namespace Pyro;

void pieChart(float diameter, int *data, int datalength) {
    float lastAngle = 0;
    for (unsigned int i = 0; i < datalength; i++) {
        int gray = map<int>(i, 0, datalength, 0, 255);
        fill(gray);
        arc(width/2.0, height/2.0,
               diameter, diameter,
               lastAngle, lastAngle+radians(data[i]), Pyro::PIE);
        lastAngle += radians(data[i]);
    }
}

int main(int argc, char **argv) {
    int angles[] = { 30, 10, 45, 35, 60, 38, 75, 67 };

    size(640, 360);

    nostroke();
    background(100);

    pieChart(300, angles, 8);

    save("output/piechart.png");

}
