//
// Created by Daniele on 21/09/2021.
//

#include "Camera.h"
#include "../Base/Log.h"

namespace Argonaut {

    Camera::Camera() {
        position = v3(0.f);
        worldUp = v3(0.f);
        up = v3(0.f);
        right = v3(0.f);
        yaw = 0.f;
        pitch = 0;
        front = v3(0.f, 0.f, -1.f);

        movementSpeed = 0;
        sensitivity = 0;
    }

    Camera::Camera(v3 pos, v3 worldUp, float yaw, float pitch, float speed, float sensitivity) {
        position = pos;
        this->worldUp = worldUp;
        this->yaw = yaw;
        this->pitch = pitch;
        front = v3(0.f, 0.f, -1.f);

        movementSpeed = speed;
        this->sensitivity = sensitivity;

        update();
    }

    void Camera::KeyControls(const bool *keys, double deltaTime) {
        float vel = movementSpeed * static_cast<float>(deltaTime);

        if (keys[GLFW_KEY_LEFT_SHIFT])
            vel *= 1.5f;
        if (keys[GLFW_KEY_W])
            position += front * vel;
        if (keys[GLFW_KEY_S])
            position -= front * vel;
        if (keys[GLFW_KEY_A])
            position -= right * vel;
        if (keys[GLFW_KEY_D])
            position += right * vel;
        if (keys[GLFW_KEY_SPACE])
            position += worldUp * vel;
        if (keys[GLFW_KEY_LEFT_CONTROL])
            position -= worldUp * vel;
    }

    void Camera::MouseControls(double xChange, double yChange) {
        xChange *= sensitivity;
        yChange *= sensitivity;

        // Constrain the yaw such that it does not become increasingly large and go beyond
        // floating point precision
        yaw = glm::mod(yaw + static_cast<float>(xChange), 360.f);
        pitch += static_cast<float>(yChange);

        if (pitch > 89.f)
            pitch = 89.f;
        else if (pitch < -89.f)
            pitch = -89.f;

        update();
    }

    m4 Camera::CalculateViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::update() {
        // Camera lookaround
        // yaw is the counter-clockwise angle starting from the x axis
        // then use trigonometry to get. Similar for the rest
        // https://learnopengl.com/img/getting-started/camera_yaw.png
        // https://learnopengl.com/img/getting-started/camera_pitch.png
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}
