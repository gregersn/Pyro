#ifndef PYRO_GLFW_H
#define PYRO_GLFW_H

#include "pyro/pyro.h"
#include "pyro/pyro_vulkan.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Pyro {
    class GLFWRunner : public Runner {
        protected:
        VulkanEngine *engine = nullptr;
        GLFWwindow *win = nullptr;

        int open_window();
        int create_renderer();
        int create_texture();

        unsigned int width;
        unsigned int height;

        public:
            GLFWRunner();
            GLFWRunner(const GLFWRunner &in);
            GLFWRunner(unsigned int width, unsigned int height);
            GLFWRunner & operator=(const GLFWRunner &in);
            ~GLFWRunner();
            int update();
            int quit();
            int init();
    };
}

#endif