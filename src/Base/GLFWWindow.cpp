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
        m_title = "Argonaut Engine";

        m_window = nullptr;

        lastX = 0;
        lastY = 0;
        xChange = 0;
        yChange = 0;
        memset(keys, 0, sizeof(bool) * 1024);
    }

    GLFWWindow::GLFWWindow(int windowWidth, int windowHeight, std::string title) {
        width = windowWidth;
        height = windowHeight;
        bufferWidth = 0;
        bufferHeight = 0;
        this->m_title = std::move(title);

        m_window = nullptr;

        lastX = 0;
        lastY = 0;
        xChange = 0;
        yChange = 0;
        memset(keys, 0, sizeof(bool) * 1024);
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

        m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
        if (m_window == nullptr)
        {
            AG_CORE_CRITICAL("Failed to create GLFW window");
            glfwTerminate();
            return false;
        }

        glfwGetFramebufferSize(m_window, &bufferWidth, &bufferHeight);
        glfwMakeContextCurrent(m_window);

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
        glfwSetWindowUserPointer(m_window, this);

        createCallbacks();

        return true;
    }

    void GLFWWindow::SetTitle(std::string windowTitle) {
        m_title = std::move(windowTitle);
        glfwSetWindowTitle(m_window, m_title.c_str());
    }

    void GLFWWindow::DisableCursor() {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void GLFWWindow::createCallbacks() {
        glfwSetKeyCallback(m_window, handleKeys);
        glfwSetCursorPosCallback(m_window, handleMouseFirst);
    }

    void GLFWWindow::handleKeys(GLFWwindow *window, int key, [[maybe_unused]] int code, int action, [[maybe_unused]] int mode) {
        GLFWWindow* win = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key < 0 || key > 1024)
            return;

//        win->keys[key] = action == GLFW_PRESS;
        if (action == GLFW_PRESS)
            win->keys[key] = true;
        else if (action == GLFW_RELEASE)
            win ->keys[key] = false;
    }

    bool GLFWWindow::GetKeyPressed(int key) const {
        if (key < 0 || key > 1024)
            return false;
        return keys[key];
    }

    /// This is to eliminate the mouse jump when first entering the screen
    /// Instead of checking on every callback if its the first mouse movement
    /// I just split it into a one time call. Seems better in my eyes.
    void GLFWWindow::handleMouseFirst(GLFWwindow *window, double xPos, double yPos) {
        GLFWWindow* win = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
        win->lastX = xPos;
        win->lastY = yPos;

        glfwSetCursorPosCallback(window, handleMouse);
        handleMouse(window, xPos, yPos);
    }

    void GLFWWindow::handleMouse(GLFWwindow *window, double xPos, double yPos) {
        GLFWWindow* win = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

        win->xChange = xPos - win->lastX;
        // reversed since y-coords range from bottom to top otherwise we would have reversed pitch
        win->yChange = win->lastY - yPos;

        win->lastX = xPos;
        win->lastY = yPos;
    }

    GLFWWindow::~GLFWWindow() {
        AG_CORE_INFO("Window {} has reached destructor.", m_title);
        glfwDestroyWindow(m_window);
        // For now we assume we only have 1 Window at any time.
        glfwTerminate();
    }

    double GLFWWindow::GetXChange() {
        double change = xChange;
        xChange = 0;
        return change;
    }

    double GLFWWindow::GetYChange() {
        double change = yChange;
        yChange = 0;
        return change;
    }
}