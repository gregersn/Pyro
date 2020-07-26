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
        create_renderer();
        create_texture();

        return 0;
    }

    int GLFWRunner::open_window() {
        win = glfwCreateWindow(this->width, this->height,
                            "Pyro", NULL, NULL);

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
        if(glfwWindowShouldClose(win)) {
            this->running = false;
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwPollEvents();
        return 0;
    }

    int GLFWRunner::quit() {
        glfwTerminate();
        return 0;
    }

}
