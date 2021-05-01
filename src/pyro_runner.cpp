#include "pyro/pyro_runner.h"
#include "pyro/pyro_sdl.h"
#include "pyro/pyro_ray.h"
#include "pyro/pyro.h"
#include <iostream>

namespace Pyro
{
    unsigned int framecount = 0;
    Runner *runner = nullptr;
    bool running;
    bool looping;

    void init()
    {
        init(Pyro::GraphicsMode::CAIRO);
    }

    void init(Pyro::GraphicsMode mode)
    {
        running = true;
        looping = true;
        switch (mode)
        {
        case Pyro::GraphicsMode::RAYLIB:
            std::cout << "Adding a rayrunner\n";
            runner = new RayRunner(width, height);
            break;

        case Pyro::GraphicsMode::CAIRO:
        default:
            std::cout << "Adding SDL runner\n";
            runner = new SDLRunner(width, height);
            break;
        }
        runner->init();
    }

    void update()
    {
        runner->update();
        running = runner->running;
        framecount++;
    }

    void quit()
    {
        runner->quit();
    }

    void loop()
    {
        looping = true;
    }

    void noloop()
    {
        looping = false;
    }

    void predraw()
    {
        runner->predraw();
        pushmatrix();
    }

    void postdraw()
    {
        popmatrix();
        runner->postdraw();
    }

    void run(void (*setup)(), void (*draw)())
    {
        run(setup, draw, Pyro::GraphicsMode::CAIRO);
    }

    void run(void (*setup)(), void (*draw)(), Pyro::GraphicsMode mode)
    {
        setup();
        init(mode);
        while (running)
        {
            if (looping)
            {
                predraw();
                draw();
                postdraw();
            }
            update();
        }
        quit();
    }
}