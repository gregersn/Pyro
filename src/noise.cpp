#include "pyro/noise.h"

#include "opensimplexnoise.h"

namespace Pyro
{
    static OpenSimplexNoise *noiseimpl;
    static bool noise_init = false;

    void noiseseed(int64_t seed)
    {
        if (noiseimpl != nullptr)
        {
            delete noiseimpl;
        }
        noiseimpl = new OpenSimplexNoise();
        noise_init = true;
    }

    double noise(double x, double y, double z, double w)
    {
        if (!noise_init)
        {
            noiseimpl = new OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return (noiseimpl->eval(x, y, z, w) + 1.0) / 2.0;
    }

    double noise(double x, double y, double z)
    {
        if (!noise_init)
        {
            noiseimpl = new OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return (noiseimpl->eval(x, y, z) + 1.0) / 2.0;
    }

    double noise(double x, double y)
    {
        if (!noise_init)
        {
            noiseimpl = new OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        return (noiseimpl->eval(x, y) + 1.0) / 2.0;
    }

    double noise(double x)
    {
        return noise(x, 0.0);
    }

}