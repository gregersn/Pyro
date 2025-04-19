#include "pyro/sdl.h"
#include "pyro/graphics_sdl.h"
#include <iostream>

namespace Pyro
{

    SDLRunner::SDLRunner(bool headless) : Runner(), headless(headless)
    {
        this->width = 0;
        this->height = 0;
        default_mode = GraphicsMode::SDL;
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
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init error: %s", SDL_GetError());
                return 1;
            }
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL initialized");
            open_window();
            create_renderer();
            create_texture();
        }
        else
        {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Running in headless mode");
        }

        return 0;
    }

    int SDLRunner::open_window()
    {
        if (!sdl_window)
        {
            sdl_window = SDL_CreateWindow("Pyro",
                                          this->width, this->height,
                                          SDL_WINDOW_OPENGL);
        }

        SDL_ShowWindow(sdl_window);
        if (sdl_window == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow Error: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    int SDLRunner::create_renderer()
    {
        if (!sdl_renderer)
        {
            sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
        }
        if (sdl_renderer == nullptr)
        {
            SDL_DestroyWindow(sdl_window);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer: %s", SDL_GetError());
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
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTexture Error: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }
        return 0;
    }

    int SDLRunner::update()
    {
        if (dynamic_cast<GraphicsSDL *>(pg) == nullptr)
        {
            SDL_UpdateTexture(sdl_texture, NULL, pg->get_data(), this->width * sizeof(uint32_t));
            SDL_RenderClear(sdl_renderer);
            SDL_RenderTexture(sdl_renderer, sdl_texture, NULL, NULL);
        }
        SDL_RenderPresent(sdl_renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                this->running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
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
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL runner quitting");
        this->running = false;
        SDL_DestroyTexture(sdl_texture);
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return 0;
    }

} // namespace Pyro
