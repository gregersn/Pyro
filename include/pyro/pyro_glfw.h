#ifndef PYRO_GLFW_H
#define PYRO_GLFW_H

#include "pyro/pyro.h"
#include <GLFW/glfw3.h>

namespace Pyro {
    class GLFWRunner : public Runner {
        protected:
        GLFWwindow *win = nullptr;

        int open_window();
        int create_renderer();
        int create_texture();

        unsigned int width;
        unsigned int height;

        static void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        public:
            GLFWRunner();
            GLFWRunner(const GLFWRunner &in);
            GLFWRunner(unsigned int width, unsigned int height);
            GLFWRunner & operator=(const GLFWRunner &in);
            ~GLFWRunner();
            int update();
            int quit();
            int init();
            int deinit();
    };
}

#endif