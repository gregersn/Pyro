#include <pyro/pyro.h>

using namespace Pyro;

int main(int argc, char **argv) {
    size(100, 100);
    
    strokeweight(1);
    line(20, 20, 80, 20);
    
    strokeweight(4);
    line(20, 40, 80, 40);

    strokeweight(10);
    line(20, 70, 80, 70);

    save("output/strokeweight.png");
    
}