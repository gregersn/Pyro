#include <iostream>
#include <pyro/pyro_glfw.h>

namespace Pyro {
    GLFWRunner::GLFWRunner() : Runner(), width(640), height(480){
    }

    GLFWRunner::GLFWRunner(unsigned int width, unsigned int height) : Runner(), width(width), height(height) {
    }

    GLFWRunner::~GLFWRunner() {

    }

    int GLFWRunner::init() {
        if(!glfwInit()) {
            std::cout << "GLFW_Init error" << std::endl;
            return 1;
        }
        std::cout << "GLFW initialized\n";
        open_window();
        this->engine = new VulkanEngine(win, width, height);
        create_renderer();
        create_texture();

        return 0;
    }

    int GLFWRunner::open_window() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        win = glfwCreateWindow(this->width, this->height,
                            "Pyro", nullptr, nullptr);

        if(!win) {
            std::cout << "GLFW_CreateWindow Error: " << std::endl;
            glfwTerminate();
            return 1;
        }

        return 0;
    }

    int GLFWRunner::create_renderer() {
        glfwMakeContextCurrent(win);
        return 0;
    }

    int GLFWRunner::create_texture() {
        return 0;
    }

    int GLFWRunner::update() {
        this->engine->draw_frame();

        if(glfwWindowShouldClose(win)) {
            this->running = false;
            this->engine->shutdown();
            return 0;
        }
        glfwSwapBuffers(win);
        glfwPollEvents();
        return 0;
    }

    int GLFWRunner::quit() {
        std::cout << "Quitting" << std::endl;
        if(this->engine != nullptr) {
            std::cout << "Cleaning up" << std::endl;
            this->engine->clean_up();
        }
        glfwDestroyWindow(win);
        glfwTerminate();
        return 0;
    }

}
