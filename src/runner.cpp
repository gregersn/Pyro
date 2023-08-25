#include "pyro/runner.h"
#include "pyro/sdl.h"
#include "pyro/pyro.h"
#include <iostream>

namespace Pyro
{
    unsigned int framecount = 0;
    bool keypressed = false;
    int key = -1;

    Runner *runner = nullptr;
    bool running;
    bool looping;
    bool _headless;

    void init(bool headless)
    {
        running = true;
        looping = true;
        _headless = headless;
        runner = new SDLRunner(width, height, _headless);
        runner->init();
    }

    void update()
    {
        if (!_headless)
            runner->update();
        running = runner->running;
        key = runner->key;
        keypressed = runner->keypressed;
        framecount++;
    }

    bool get_keypressed()
    {
        return keypressed;
    }

    int get_key()
    {
        return key;
    }

    void quit()
    {
        std::cout << "Quitting\n";
        runner->quit();
    }

    void stop()
    {
        runner->running = false;
    }

    void loop()
    {
        looping = true;
    }

    void noloop()
    {
        looping = false;
    }

    void run(void (*setup)(), void (*draw)(), bool headless)
    {
        std::cout << "Calling setup\n";
        setup();
        std::cout << "Calling init\n";
        init(headless);

        std::cout << "Starting run loop\n";
        while (running)
        {
            if (looping)
            {
                pushmatrix();
                draw();
                popmatrix();
            }
            update();
        }
        std::cout << "Exiting run loop\n";
        quit();
    }
}
