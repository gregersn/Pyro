#include "pyro/noise.h"

#include "opensimplexnoise.h"

#include <cmath>

namespace Pyro
{
    static OpenSimplexNoise noiseimpl;
    static bool noise_init = false;
    static unsigned int noise_lod = 4;
    static float noise_falloff = .5f;

    void noiseseed(long seed)
    {
        noiseimpl = OpenSimplexNoise(seed);
        noise_init = true;
    }

    void noisedetail(unsigned int lod, float falloff = 0.5f)
    {
        noise_lod = lod;
        noise_falloff = falloff;
    }

    double noise(double x, double y, double z, double w)
    {
        if (!noise_init)
        {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }
        float noise = 0;
        for (unsigned int level = 0; level < noise_lod; level++)
        {
            unsigned int scale = level + 1;
            noise += noiseimpl.eval(x * scale, y * scale, z * scale, w * scale) * pow(noise_falloff, scale);
        }

        return (noise + 1.0) / 2.0;
    }

    double noise(double x, double y, double z)
    {
        if (!noise_init)
        {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        float noise = 0;
        for (unsigned int level = 0; level < noise_lod; level++)
        {
            unsigned int scale = level + 1;
            noise += noiseimpl.eval(x * scale, y * scale, z * scale) * pow(noise_falloff, scale);
        }

        return (noise + 1.0) / 2.0;
    }

    double noise(double x, double y)
    {
        if (!noise_init)
        {
            noiseimpl = OpenSimplexNoise(123456789L);
            noise_init = true;
        }

        float noise = 0;
        for (unsigned int level = 0; level < noise_lod; level++)
        {
            unsigned int scale = level + 1;
            noise += noiseimpl.eval(x * scale, y * scale) * pow(noise_falloff, scale);
        }

        return (noise + 1.0) / 2.0;
    }

    double noise(double x)
    {
        return noise(x, 0.0);
    }

} // namespace Pyro