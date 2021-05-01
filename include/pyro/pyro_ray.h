#ifndef PYRO_RAY_H
#define PYRO_RAY_H

#include "pyro/pyro.h"

namespace Pyro
{
    class RayRunner : public Runner
    {
    protected:
        int open_window();
        int create_renderer();
        int create_texture();

        unsigned int width;
        unsigned int height;

    public:
        RayRunner();
        RayRunner(const RayRunner &in);
        RayRunner(unsigned int width, unsigned int height);
        RayRunner &operator=(const RayRunner &in);
        ~RayRunner();
        int update();
        int quit();
        int init();
        void predraw();
        void postdraw();
    };
}

#endif