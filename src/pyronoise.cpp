#include "pyro/pyronoise.h"

#include "opensimplexnoise.h"

namespace Pyro {
    static OpenSimplexNoise noiseimpl;
    static bool noise_init = false;
    double noise(double x, double y, double z, double w) {
        if(!noise_init) {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return noiseimpl.eval(x, y, z, w);
    }

    double noise(double x, double y, double z) {
        if(!noise_init) {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return noiseimpl.eval(x, y, z);
    }

    double noise(double x, double y) {
        if(!noise_init) {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return noiseimpl.eval(x, y);
    }

    double noise(double x) {
        return noise(x, 0.0);
    }

}