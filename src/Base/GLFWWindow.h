//
// Created by Daniele on 17/09/2021.
//

#ifndef ENGINE_GLFWWINDOW_H
#define ENGINE_GLFWWINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Log.h"

namespace Argonaut {
    class GLFWWindow {
    public:
        GLFWWindow();
        GLFWWindow(int windowWidth, int windowHeight, std::string title);
        ~GLFWWindow();

        bool Initialize();

        [[nodiscard]]
        bool ShouldClose() const {return glfwWindowShouldClose(window); }
        void SetTitle(std::string windowTitle);
        void SwapBuffers() { glfwSwapBuffers(window); }
        void DisableCursor();

        [[nodiscard]]
        int GetBufferWidth() const { return bufferWidth; }
        [[nodiscard]]
        int GetBufferHeight() const { return bufferHeight; }
        [[nodiscard]]
        double GetXChange() const { return xChange; }
        [[nodiscard]]
        double GetYChange() const {return yChange; }
        [[nodiscard]]
        bool GetKeyPressed(int key) const;

        // TODO Remove this
        /**
        * If you use this function it is most likely a bad idea. Will be replaced with custom
        * window functions
        */
        [[nodiscard]]
        GLFWwindow* GetMainWindow() const {return window;}

    private:
        std::string title;
        GLFWwindow *window;

        int width;
        int bufferWidth;
        int height;
        int bufferHeight;

        // TODO move this to an event based system
        bool keys[1024];

        double lastX;
        double lastY;
        double xChange;
        double yChange;

        void createCallbacks();

        // TODO replace all of this with event based systems!
        static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
        static void handleMouseFirst(GLFWwindow* window, double xPos, double yPos);
        static void handleMouse(GLFWwindow* window, double xPos, double yPos);
    };
}


#endif //ENGINE_GLFWWINDOW_H
