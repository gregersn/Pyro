#include "pyro/graphics_sdl.h"
#include "pyro/constants.h"
#include "pyro/font_impl.h"
#include "pyro/utils.h"
#include "pyro/sdl.h"

#include <iostream>

namespace Pyro
{
    SDL_Window *sdl_window{nullptr};
    SDL_Renderer *sdl_renderer{nullptr};

    GraphicsSDL::GraphicsSDL(unsigned int width, unsigned int height, GraphicsMode mode, std::filesystem::path filename) : Graphics(width, height, filename)
    {
        this->mode = mode;
        int render_driver_count = SDL_GetNumRenderDrivers();
        SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Available renderers: %i", render_driver_count);
        for (int i{0}; i < render_driver_count; i++)
        {
            SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, SDL_GetRenderDriver(i));
        }
    }

    GraphicsSDL::~GraphicsSDL()
    {
        if (sdl_renderer)
        {
            SDL_DestroyRenderer(sdl_renderer);
        }

        if (sdl_window)
        {
            SDL_DestroyWindow(sdl_window);
        }
    }

    void GraphicsSDL::init()
    {
        Image::init();
        sdl_window = SDL_CreateWindow("Pyro window", this->width(), this->height(), SDL_WINDOW_HIDDEN);
        if (!sdl_window)
        {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not create window: %s", SDL_GetError());
        }
        sdl_renderer = SDL_CreateRenderer(sdl_window, NULL);
        if (!sdl_renderer)
        {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not create renderer: %s", SDL_GetError());
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Created renderer: %s", SDL_GetRendererName(sdl_renderer));
        Graphics::init();
    }

    uint32_t GraphicsSDL::get(int x, int y)
    {
        pixels = SDL_RenderReadPixels(sdl_renderer, NULL);
        return static_cast<uint32_t *>(pixels->pixels)[y * this->width() + x];
    }

    void GraphicsSDL::save(const std::filesystem::path &filename, unsigned int dpi)
    {
        pixels = SDL_RenderReadPixels(sdl_renderer, NULL);
        Image *out = createimage(this->width(), this->height(), RGBA);
        for (unsigned int i = 0; i < this->width() * this->height(); i++)
        {
            static_cast<uint32_t *>(out->get_data())[i] = static_cast<uint32_t *>(pixels->pixels)[i];
        }
        out->save(filename, dpi);
        SDL_DestroySurface(pixels);
    }

    void GraphicsSDL::background(float r, float g, float b, float a)
    {
        SDL_SetRenderDrawColorFloat(sdl_renderer,
                                    r,
                                    g,
                                    b,
                                    a);
        SDL_RenderClear(sdl_renderer);
    }

    void GraphicsSDL::point(Vector p)
    {
        SDL_RenderPoint(sdl_renderer, p.x, p.y);
    }

    void GraphicsSDL::point(float x, float y)
    {
        this->point(Vector(x, y));
    }

    void GraphicsSDL::line(Vector p0, Vector p1)
    {
        if (this->style.strokeweight() != 1)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Unsupported line weight");
        }
        if (this->style.stroke_enabled())
        {
            SDL_SetRenderDrawColorFloat(sdl_renderer,
                                        this->style.stroke().r(),
                                        this->style.stroke().g(),
                                        this->style.stroke().b(),
                                        this->style.stroke().a());

            SDL_RenderLine(sdl_renderer,
                           this->transformer.screen_x(p0.x - .5, p0.y - .5),
                           this->transformer.screen_y(p0.x - .5, p0.y - .5),
                           this->transformer.screen_x(p1.x - .5, p1.y - .5),
                           this->transformer.screen_y(p1.x - .5, p1.y - .5));
        }
    };

    void GraphicsSDL::shape(Shape s, float x, float y)
    {
        if (this->style.fill_enabled())
        {
            uint16_t num_points{0};
            for (size_t contour_index{0}; contour_index < s.getpoints().size(); contour_index++)
            {
                std::vector<Pyro::Vector> contour_points{s.getpoints()[contour_index]};
                num_points += contour_points.size();
            }

            SDL_Vertex *vert = new SDL_Vertex[num_points];

            uint32_t vertindex{0};
            for (size_t contour_index{0}; contour_index < s.getpoints().size(); contour_index++)
            {
                std::vector<Pyro::Vector> contour_points{s.getpoints()[contour_index]};

                for (size_t i{0}; i < contour_points.size(); i++)
                {
                    vert[vertindex].position.x = contour_points[i].x + screen_x(x, y);
                    vert[vertindex].position.y = contour_points[i].y + screen_y(x, y);
                    vert[vertindex].color.r = this->style.fill().r();
                    vert[vertindex].color.g = this->style.fill().g();
                    vert[vertindex].color.b = this->style.fill().b();
                    vert[vertindex].color.a = this->style.fill().a();
                    vertindex++;
                }
            }
            std::vector<int32_t> indices = s.indices();
            uint32_t index_count = indices.size();
            SDL_RenderGeometry(sdl_renderer,
                               NULL,
                               vert,
                               num_points,
                               indices.data(),
                               index_count);

            delete[] vert;
        }
        if (this->style.stroke_enabled())
        {
            if (this->style.strokeweight() != 1)
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Unsupported line weight");
            }

            SDL_SetRenderDrawColorFloat(sdl_renderer,
                                        this->style.stroke().r(),
                                        this->style.stroke().g(),
                                        this->style.stroke().b(),
                                        this->style.stroke().a());

            for (size_t contour_index{0}; contour_index < s.getpoints().size(); contour_index++)
            {
                std::vector<Pyro::Vector> contour_points{s.getpoints()[contour_index]};
                uint16_t num_points = contour_points.size();
                if (s.close == CLOSE)
                {
                    num_points += 1;
                }
                SDL_FPoint *lines = new SDL_FPoint[num_points];

                for (size_t i{0}; i < contour_points.size(); i++)
                {
                    lines[i].x = contour_points[i].x + screen_x(x, y);
                    lines[i].y = contour_points[i].y + screen_y(x, y);
                }
                if (s.close == CLOSE)
                {
                    lines[num_points - 1].x = contour_points[0].x + screen_x(x, y);
                    lines[num_points - 1].y = contour_points[0].y + screen_y(x, y);
                }
                SDL_RenderLines(sdl_renderer,
                                lines,
                                num_points);
                delete[] lines;
            }
        }
    }

    void GraphicsSDL::enddraw()
    {
    }

    void GraphicsSDL::blendmode(int mode)
    {
        SDL_BlendMode sdl_mode = SDL_BLENDMODE_NONE;
        switch (mode)
        {
        case Pyro::BLEND:
            sdl_mode = SDL_BLENDMODE_BLEND;
            break;
        case Pyro::REPLACE:
            sdl_mode = SDL_BLENDMODE_NONE;
            break;
        case Pyro::ADD:
            sdl_mode = SDL_BLENDMODE_ADD;
            break;
        case Pyro::LIGHTEST:
            sdl_mode = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_MAXIMUM,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_MAXIMUM);
            break;
        case Pyro::DARKEST:
            sdl_mode = SDL_ComposeCustomBlendMode(
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_MINIMUM,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDFACTOR_ONE,
                SDL_BLENDOPERATION_MINIMUM);
            break;
        case Pyro::DIFFERENCE:
            sdl_mode = SDL_BLENDMODE_INVALID;
            break;
        case Pyro::EXCLUSION:
            sdl_mode = SDL_BLENDMODE_INVALID;
            break;
        case Pyro::MULTIPLY:
            sdl_mode = SDL_BLENDMODE_MUL;
            break;
        case Pyro::SCREEN:
        default:
            sdl_mode = SDL_BLENDMODE_INVALID;
            break;
        }
        if (!SDL_SetRenderDrawBlendMode(sdl_renderer, sdl_mode))
        {
            SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Could not set blendmode: %s", SDL_GetError());
        }
    }
    void GraphicsSDL::image_impl(Image *img, float x, float y)
    {
        if (dynamic_cast<GraphicsSDL *>(img) != nullptr)
        {
            GraphicsSDL *source = static_cast<GraphicsSDL *>(img);
            SDL_Surface *surface{nullptr};
            surface = SDL_RenderReadPixels(sdl_renderer, NULL);

            SDL_Texture *texture{nullptr};
            texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
            SDL_DestroySurface(surface);

            SDL_FRect dest;
            dest.x = x;
            dest.y = y;
            dest.w = source->width();
            dest.h = source->height();

            SDL_RenderTexture(sdl_renderer,
                              texture,
                              NULL,
                              &dest);
            SDL_DestroyTexture(texture);
        }
        else
        {
            throw std::logic_error("Image image not implemented");
        }
    }

} // namespace Pyro
