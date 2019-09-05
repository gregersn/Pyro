#include <pyro/pyromath.h>
#include <pyro/pyroconstants.h>

#include <iostream>

namespace Pyro {
    float radians(float degree) { 
        return degree * M_PI / 180.0; 
    }

    float degrees(float radians) {
        return radians * 180.0 / M_PI;
    }

    int random(int range) {
        return (int)(random() * range);
    }

    int random(int low, int high) {
        assert(high > low);
        return (int)((random() * (high - low)) + low);
    }
    
    unsigned int random(unsigned int range) {
        return (unsigned int)(random() * range);
    }

    unsigned int random(unsigned int low, unsigned int high) {
        assert(high > low);
        return (unsigned int)((random() * (high - low)) + low);
    }
    
    double random(double range) {
        return random() * range;
    }

    double random(double low, double high) {
        assert(high > low);
        return (random() * (high - low)) + low;
    }

    double random() {
        return rng_dist(rng);
    }

    void randomseed(unsigned int seed) {
        rng.seed(seed);
    }

}
