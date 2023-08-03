#include "pyro/sdl.h"
#include <iostream>

namespace Pyro
{
    SDLRunner::SDLRunner() : Runner(), width(640), height(480)
    {
    }

    SDLRunner::SDLRunner(unsigned int width, unsigned int height, bool headless) : Runner(), width(width), height(height), headless(headless)
    {
    }

    SDLRunner::~SDLRunner()
    {
    }

    int SDLRunner::init()
    {
        if (!headless)
        {
            if (SDL_Init(SDL_INIT_VIDEO) != 0)
            {
                std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
                return 1;
            }
            std::cout << "SDL initialized\n";
            open_window();
            create_renderer();
            create_texture();
        }
        else
        {
            std::cout << "Running in headless mode\n";
        }

        return 0;
    }

    int SDLRunner::open_window()
    {
        win = SDL_CreateWindow("Pyro",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               this->width, this->height,
                               SDL_WINDOW_OPENGL);

        if (win == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    int SDLRunner::create_renderer()
    {
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (ren == nullptr)
        {
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        return 0;
    }

    int SDLRunner::create_texture()
    {
        tex = SDL_CreateTexture(ren,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                this->width, this->height);
        if (tex == nullptr)
        {
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        return 0;
    }

    int SDLRunner::update()
    {
        SDL_UpdateTexture(tex, NULL, pg->get_data(), width * sizeof(uint32_t));
        SDL_Event e;
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                this->running = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    this->running = false;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                // this->running = false;
            }
        }
        return 0;
    }

    int SDLRunner::quit()
    {
        std::cout << "SDL runner quitting\n";
        this->running = false;
        SDL_DestroyTexture(tex);
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 0;
    }

}
