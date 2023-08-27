#include "pyro/runner.h"
#include "pyro/sdl.h"
#include "pyro/pyro.h"
#include <iostream>

namespace Pyro
{
    unsigned int framecount{};
    bool keypressed{};
    int key{-1};

    bool mousepressed{};
    int mousebutton{-1};
    int mousex{};
    int mousey{};
    int pmousex{};
    int pmousey{};

    Runner *runner{};
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

        mousex = runner->mousex;
        mousey = runner->mousey;
        pmousex = runner->pmousex;
        pmousey = runner->pmousey;

        mousepressed = runner->mousepressed;
        mousebutton = runner->mousebutton;

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

    bool get_mousepressed()
    {
        return mousepressed;
    }

    int get_mousebutton()
    {
        return mousebutton;
    }

    int get_framecount()
    {
        return framecount;
    }

    int get_mousex() { return mousex; }
    int get_mousey() { return mousey; }
    int get_pmousex() { return pmousex; }
    int get_pmousey() { return pmousey; }

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
