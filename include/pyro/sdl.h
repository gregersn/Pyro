#ifndef PYRO_SDL_H
#define PYRO_SDL_H

#include "pyro/pyro.h"
#include <SDL2/SDL.h>

namespace Pyro
{
    class SDLRunner : public Runner
    {
    protected:
        SDL_Window *sdl_window{nullptr};
        SDL_Renderer *sdl_renderer{nullptr};
        SDL_Texture *sdl_texture{nullptr};

        int open_window();
        int create_renderer();
        int create_texture();

        bool headless;
        unsigned int width;
        unsigned int height;

    public:
        SDLRunner(bool headless = false);
        SDLRunner(const SDLRunner &in);
        SDLRunner &operator=(const SDLRunner &in);
        ~SDLRunner();
        int update();
        int quit();
        int init(unsigned int width, unsigned int height);
    };
}

#endif
