#ifndef PYROGRAPHICS_SDL_H
#define PYROGRAPHICS_SDL_H

#include "graphics.h"

#include <SDL3/SDL.h>

namespace Pyro
{
    class GraphicsSDL : public Graphics
    {
      private:
        SDL_Surface *pixels{nullptr};

      public:
        GraphicsSDL(unsigned int width, unsigned int height,
                    GraphicsMode mode = GraphicsMode::SDL,
                    std::filesystem::path filename = "");
        ~GraphicsSDL() override;
        void init() override;
        void save(const std::filesystem::path &filename, unsigned int dpi) override;
        void blendmode(int mode) override;
        void image_impl(Image *img, float x, float y) override;

        uint32_t get(int x, int y) override;

        void point(float x, float y) override;
        void point(Vector p) override;

        void line(Vector p0, Vector p1) override;
        void line(float x0, float y0, float x1, float y1) override
        {
            this->line(Vector(x0, y0), Vector(x1, y1));
        };

        void background(float r, float g, float b, float a) override;
        void shape(Shape s, float x, float y) override;
        void enddraw() override;
    };
} // namespace Pyro

#endif
