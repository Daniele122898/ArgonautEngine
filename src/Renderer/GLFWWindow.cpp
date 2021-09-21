//
// Created by Daniele on 17/09/2021.
//

#include "GLFWWindow.h"

#include <utility>
namespace Argonaut {

    GLFWWindow::GLFWWindow() {
        width = 800;
        height = 600;
        bufferWidth = 0;
        bufferHeight = 0;
        title = "Argonaut Engine";

        window = nullptr;
    }

    GLFWWindow::GLFWWindow(int windowWidth, int windowHeight, std::string title) {
        width = windowWidth;
        height = windowHeight;
        bufferWidth = 0;
        bufferHeight = 0;
        this->title = std::move(title);

        window = nullptr;
    }

    bool GLFWWindow::Initialize() {
        if (!glfwInit()) {
            AG_CORE_CRITICAL("Failed to initialize glfw");
            glfwTerminate();
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            AG_CORE_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            return false;
        }

        glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            AG_CORE_CRITICAL("Failed to initialize GLAD");
            glfwTerminate();
            return false;
        }

        glEnable(GL_DEPTH_TEST);

        // Set width and height to the actual size inside the window we got when getting the buffer sizes
        glViewport(0, 0, bufferWidth, bufferHeight);
        // So the window has a pointer to this class
        glfwSetWindowUserPointer(window, this);

        return true;
    }

    void GLFWWindow::SetTitle(std::string windowTitle) {
        title = std::move(windowTitle);
        glfwSetWindowTitle(window, title.c_str());
    }

    void GLFWWindow::DisableCursor() {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}