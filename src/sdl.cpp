#include "pyro/sdl.h"
#include <iostream>

namespace Pyro
{
    SDLRunner::SDLRunner(bool headless) : Runner(), headless(headless)
    {
        this->width = 0;
        this->height = 0;
    }

    SDLRunner::~SDLRunner()
    {
    }

    int SDLRunner::init(unsigned int width, unsigned int height)
    {
        this->width = width;
        this->height = height;

        if (!headless)
        {
            if (!SDL_Init(SDL_INIT_VIDEO))
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
        sdl_window = SDL_CreateWindow("Pyro",
                                      this->width, this->height,
                                      SDL_WINDOW_OPENGL);

        if (sdl_window == nullptr)
        {
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    int SDLRunner::create_renderer()
    {
        sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
        if (sdl_renderer == nullptr)
        {
            SDL_DestroyWindow(sdl_window);
            std::cout << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        return 0;
    }

    int SDLRunner::create_texture()
    {
        sdl_texture = SDL_CreateTexture(sdl_renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        this->width, this->height);
        if (sdl_texture == nullptr)
        {
            SDL_DestroyRenderer(sdl_renderer);
            SDL_DestroyWindow(sdl_window);
            std::cout << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }
        return 0;
    }

    int SDLRunner::update()
    {
        SDL_UpdateTexture(sdl_texture, NULL, pg->get_data(), this->width * sizeof(uint32_t));
        SDL_Event e;
        SDL_RenderClear(sdl_renderer);
        SDL_RenderTexture(sdl_renderer, sdl_texture, NULL, NULL);
        SDL_RenderPresent(sdl_renderer);

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                this->running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                // std::cout << "Keydown" << std::endl;
                this->keypressed = true;
                this->key = e.key.key;
                if (e.key.mod == SDLK_ESCAPE)
                    this->running = false;

                if (this->keypressed_cb != nullptr)
                {
                    this->keypressed_cb();
                }
                break;

            case SDL_EVENT_KEY_UP:
                // std::cout << "Keyup" << std::endl;
                this->keypressed = false;
                this->key = e.key.key;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                this->mousepressed = true;
                this->mousebutton = e.button.button;
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                this->mousepressed = false;
                this->mousebutton = e.button.button;
                break;

            case SDL_EVENT_MOUSE_MOTION:
                pmousex = mousex;
                pmousey = mousey;
                mousex = e.motion.x;
                mousey = e.motion.y;
                break;

            default:
                break;
            }
        }
        return 0;
    }

    int SDLRunner::quit()
    {
        std::cout << "SDL runner quitting\n";
        this->running = false;
        SDL_DestroyTexture(sdl_texture);
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return 0;
    }

} // namespace Pyro
