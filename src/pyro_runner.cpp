#include "pyro/pyro_runner.h"
#include "pyro/pyro_sdl.h"
#include "pyro/pyro_glfw.h"
#include "pyro/pyro.h"
#include <iostream>

namespace Pyro {
    unsigned int framecount = 0;
    Runner *runner = nullptr;
    bool running;
    bool looping;

    void init(RUNNER _r) {
        running = true;
        looping = true;
        switch(_r) {
            case RUNNER::GLFW:
                runner = new GLFWRunner(width, height);
            break;
            case RUNNER::SDL:
            default:
                runner = new SDLRunner(width, height);
            break;
        }
        runner->init();
    }

    void update() {
        runner->update();
        running = runner->running;
        framecount++;
    }

    void deinit() {
        runner->deinit();
    }

    void quit() {
        runner->quit();
    }

    void loop() {
        looping = true;
    }

    void noloop() {
        looping = false;
    }

    void run(void (*setup)(), void (*draw)()) {
        run(setup, draw, RUNNER::SDL);
    }

    void keydown(int key) {
        if(key == KEYBOARD_ESCAPE) {
            runner->quit();
            return;
        }
        std::cout << "Key pressed " << static_cast<int>(key) << std::endl;
    }

    void run(void (*setup)(), void (*draw)(), RUNNER runner) {
        setup();
        init(runner);
        while(running) {
            if(looping) {
                pushmatrix();
                draw();
                popmatrix();
            }
            update();
        }
        deinit();

    }
}