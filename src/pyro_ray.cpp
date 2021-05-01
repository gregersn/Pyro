#include <iostream>

#include "pyro/pyro_ray.h"

namespace RayLib
{
#include "raylib.h"
}

namespace Pyro
{
    RayRunner::RayRunner() : Runner(), width(640), height(480)
    {
    }

    RayRunner::RayRunner(unsigned int width, unsigned int height) : Runner(), width(width), height(height)
    {
    }

    RayRunner::~RayRunner()
    {
    }

    int RayRunner::init()
    {
        RayLib::InitWindow(this->width, this->height, "Raylib window");
        RayLib::SetTargetFPS(60);
        //ToggleFullscreen();
        return 0;
    }

    int RayRunner::open_window()
    {

        return 0;
    }

    int RayRunner::create_renderer()
    {
    }

    int RayRunner::create_texture()
    {
    }

    int RayRunner::update()
    {
        return 0;
    }

    int RayRunner::quit()
    {
        RayLib::CloseWindow();
        return 0;
    }

    void RayRunner::predraw()
    {
        RayLib::BeginDrawing();
    }

    void RayRunner::postdraw()
    {
        RayLib::EndDrawing();
    }

}
