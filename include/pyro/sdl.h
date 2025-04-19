#ifndef PYRO_SDL_H
#define PYRO_SDL_H

#include "pyro/pyro.h"
#include <SDL3/SDL.h>

namespace Pyro
{
    extern SDL_Window *sdl_window;
    extern SDL_Renderer *sdl_renderer;

    class SDLRunner : public Runner
    {
      protected:
        SDL_Texture *sdl_texture{nullptr};

        int open_window();
        int create_renderer();
        int create_texture();

        bool headless;
        unsigned int width;
        unsigned int height;

      public:
        explicit SDLRunner(bool headless = false);
        SDLRunner(const SDLRunner &in);
        SDLRunner &operator=(const SDLRunner &in);
        ~SDLRunner() override;
        int update() override;
        int quit() override;
        int init(unsigned int width, unsigned int height) override;
    };
} // namespace Pyro

#endif
