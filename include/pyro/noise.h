#ifndef PYRONOISE_H
#define PYRONOISE_H

namespace Pyro
{
    void noiseseed(long seed);
    double noise(double x, double y);
    double noise(double x, double y, double z);
    double noise(double x, double y, double z, double w);
}

#endif