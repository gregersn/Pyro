#include "pyro/runner.h"
#include "pyro/sdl.h"
#include "pyro/pyro.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace Pyro
{
    unsigned int framecount = 0;
    bool keypressed = false;
    int key = -1;

    bool mousepressed = false;
    int mousebutton = -1;
    int mousex = 0;
    int mousey = 0;
    int pmousex = 0;
    int pmousey = 0;
    float framerate = 60.0f;

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

        mousex = runner->mousex;
        mousey = runner->mousey;
        pmousex = runner->pmousex;
        pmousey = runner->pmousey;

        mousepressed = runner->mousepressed;
        if (mousepressed && runner->mousepressed_cb != nullptr)
        {
            runner->mousepressed_cb();
        }
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

    void set_keypressed(std::function<void()> keypressed)
    {
        std::cout << "in set keypressed" << std::endl;
        runner->set_keypressed(keypressed);
    }

    bool get_mousepressed()
    {
        return mousepressed;
    }

    void set_mousepressed(std::function<void()> mousepressed)
    {
        runner->set_mousepressed(mousepressed);
    }

    int get_mousebutton()
    {
        return mousebutton;
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

    void run(const std::function<void()> &setup, const std::function<void()> &draw, bool headless)
    {
        presetup(headless);
        std::cout << "Calling setup\n";
        setup();
        std::cout << "Calling init\n";
        init(Pyro::width, Pyro::height);

        std::cout << "Starting run loop\n";
        double frame_time = 1000.0f / framerate;
        while (running)
        {
            double start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if (looping)
            {
                pushmatrix();
                draw();
                popmatrix();
            }
            update();
            double end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto diff_time = end_time - start_time;
            if (diff_time < frame_time)
            {
                std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(frame_time - diff_time));
            }
        }
        std::cout << "Exiting run loop\n";
        quit();
    }

    void Runner::set_keypressed(std::function<void()> keypressed_cb)
    {
        std::cout << "Setting keypressed in the runner" << std::endl;
        // std::cout << "Incoming value is: " << reinterpret_cast<void *>(keypressed_cb) << std::endl;
        this->keypressed_cb = keypressed_cb;
        std::cout << "Done setting" << std::endl;
        // keypressed_cb();
    }
    void Runner::set_mousepressed(std::function<void()> mousepressed_cb)
    {
        std::cout << "Setting mousepressed in the runner" << std::endl;
        // std::cout << "Incoming value is: " << reinterpret_cast<void *>(mousepressed_cb) << std::endl;
        this->mousepressed_cb = mousepressed_cb;
        std::cout << "Done setting" << std::endl;
        // mousepressed_cb();
    }
} // namespace Pyro
