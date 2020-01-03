#ifndef PYRO_SDL_H
#define PYRO_SDL_H

#include <functional>
#include "pyro.h"
#include <SDL2/SDL.h>

namespace Pyro {
    class PyroRunner {
        SDL_Window *win = nullptr;
        SDL_Renderer *ren = nullptr;
        SDL_Texture *tex = nullptr;

        int open_window();
        int create_renderer();
        int create_texture();

        unsigned int width;
        unsigned int height;

        public:
            PyroRunner();
            PyroRunner(unsigned int width, unsigned int height);
            ~PyroRunner();
            int update();
            int quit();
            int init();
            bool running;
    };

    extern unsigned int framecount;

    void init();
    void update();
    void quit();
    void run(std::function<void()>  setup, std::function<void()>  draw);
    void noloop();
    void loop();

    extern bool running;
    extern PyroRunner *pyro;

}


#endif