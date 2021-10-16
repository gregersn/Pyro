#ifndef PYRO_SDL_H
#define PYRO_SDL_H

#include "pyro/pyro.h"
#include <SDL2/SDL.h>

namespace Pyro
{
    class SDLRunner : public Runner
    {
    protected:
        SDL_Window *win = nullptr;
        SDL_Renderer *ren = nullptr;
        SDL_Texture *tex = nullptr;

        int open_window();
        int create_renderer();
        int create_texture();

        unsigned int width;
        unsigned int height;

    public:
        SDLRunner();
        SDLRunner(const SDLRunner &in);
        SDLRunner(unsigned int width, unsigned int height);
        SDLRunner &operator=(const SDLRunner &in);
        ~SDLRunner();
        int update();
        int quit();
        int init();
    };
}

#endif