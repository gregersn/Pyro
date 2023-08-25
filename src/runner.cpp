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

    void presetup(bool headless)
    {
        _headless = headless;
        runner = new SDLRunner(_headless);
    }

    void init(unsigned int width, unsigned int height)
    {
        running = true;
        looping = true;
        runner->init(width, height);
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

    void set_keypressed(void (*keypressed)())
    {
        std::cout << "in set keypressed" << std::endl;
        runner->set_keypressed(keypressed);
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
        presetup(headless);
        std::cout << "Calling setup\n";
        setup();
        std::cout << "Calling init\n";
        init(Pyro::width, Pyro::height);

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

    void Runner::set_keypressed(void (*keypressedkb)())
    {
        std::cout << "Setting keypressed in the runner" << std::endl;
        std::cout << "Incoming value is: " << reinterpret_cast<void *>(keypressedkb) << std::endl;
        this->keypressed_cb = keypressedkb;
        std::cout << "Done setting" << std::endl;
        // keypressedkb();
    }
}
