//
// Created by Daniele on 21/09/2021.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Base/Types.h"


namespace Argonaut {
    class Camera {
    public:
        Camera();
        Camera(v3 pos, v3 worldUp, float yaw,
               float pitch, float speed, float sensitivity);

        void KeyControls(const bool* keys, double deltaTime);
        void MouseControls(double xChange, double yChange);

        v3 GetPosition() { return position; }
        v3 GetDirection() {return front; }

        m4 CalculateViewMatrix();

    private:
        v3 position;
        v3 front;
        v3 up;
        v3 right;
        v3 worldUp;

        float yaw;
        float pitch;

        float movementSpeed;
        float sensitivity;

        void update();
    };
}


#endif //ENGINE_CAMERA_H
