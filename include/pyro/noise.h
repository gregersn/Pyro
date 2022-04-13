#ifndef PYRONOISE_H
#define PYRONOISE_H

namespace Pyro
{
    void noiseseed(long seed);
    void noisedetail(unsigned int lod, float falloff);
    double noise(double x, double y);
    double noise(double x, double y, double z);
    double noise(double x, double y, double z, double w);
}

#endif