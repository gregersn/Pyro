#include "pyro/pyro_runner.h"
#include "pyro/pyro_sdl.h"
#include "pyro/pyro.h"
#include <iostream>

namespace Pyro {
    unsigned int framecount = 0;
    Runner *runner = nullptr;
    bool running;
    bool looping;

    void init() {
        running = true;
        looping = true;
        runner = new SDLRunner(width, height);
        runner->init();
    }

    void update() {
        runner->update();
        running = runner->running;
        framecount++;
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
        setup();
        init();
        while(running) {
            if(looping) {
                pushmatrix();
                draw();
                popmatrix();
            }
            update();
        }
        quit();

    }
}