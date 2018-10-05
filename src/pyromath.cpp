#include <pyromath.h>
#include <pyroconstants.h>

namespace Pyro {
    float radians(float degree) { 
        return degree * M_PI / 180.0; 
    }

    int random(int range) {
        return (int)random() * range;
    }

    int random(int low, int high) {
        assert(high > low);
        return (int)(random() * (high - low)) + low;
    }
    
    unsigned int random(unsigned int range) {
        return (unsigned int)random() * range;
    }

    unsigned int random(unsigned int low, unsigned int high) {
        assert(high > low);
        return (unsigned int)(random() * (high - low)) + low;
    }
    
    float random(float range) {
        return random() * range;
    }

    float random(float low, float high) {
        assert(high > low);
        return (random() * (high - low)) + low;
    }

    float random() {
        return rng_dist(rng);
    }

    void randomseed(unsigned int seed) {
        rng.seed(seed);
    }

}