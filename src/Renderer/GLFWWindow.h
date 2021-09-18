//
// Created by Daniele on 17/09/2021.
//

#ifndef ENGINE_GLFWWINDOW_H
#define ENGINE_GLFWWINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "../Base/Log.h"

namespace Argonaut {
    class GLFWWindow {
    public:
        GLFWWindow();
        GLFWWindow(int windowWidth, int windowHeight, std::string title);

        int Initialize();


    private:
        int width;
        int bufferWidth;
        int height;
        int bufferHeight;

        std::string title;

        GLFWwindow *window;
    };
}


#endif //ENGINE_GLFWWINDOW_H
