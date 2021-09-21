#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Base/Types.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Base/GLFWWindow.h"
#include "Base/Log.h"

// globals
const int ARGONAUT_WINDOW_WIDTH = 1280;
const int ARGONAUT_WINDOW_HEIGHT = 720;

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double deltaTime);
std::string readFileToString(std::string const& path);
void mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos);

Argonaut::v3 cameraPos;
Argonaut::v3 cameraTarget;
Argonaut::v3 cameraUp;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

double lastX = ARGONAUT_WINDOW_WIDTH/2., lastY = ARGONAUT_WINDOW_HEIGHT/2.;
float yaw = -90.f;
float pitch = 0.f;

int main()
{
    using namespace Argonaut;

    Argonaut::Log::Init();

    AG_CORE_INFO("Initializing Argonaut Engine");

    Argonaut::GLFWWindow agWindow = Argonaut::GLFWWindow(ARGONAUT_WINDOW_WIDTH, ARGONAUT_WINDOW_HEIGHT, "Argonaut Engine");
    if (!agWindow.Initialize()) {
        AG_CORE_CRITICAL("Failed to initialize Argonaut Window");
        glfwTerminate();
        return 1;
    }

    agWindow.DisableCursor();
    // TODO Move to camera class and window class!
    glfwSetCursorPosCallback(agWindow.GetMainWindow(), mouse_callback);

	// Building Shader
    Argonaut::Shader shader("src/Renderer/Shaders/Simple/simple_vert.glsl",
                  "src/Renderer/Shaders/Simple/simple_frag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    uint indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    uint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
//    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Create Texture
    Argonaut::Texture texture("assets/Textures/Simple/container.jpg", GL_RGB);
    texture.LoadTexture();

    Argonaut::Texture texture2("assets/Textures/Simple/awesomeface.png", GL_RGBA);
    texture2.LoadTexture(true);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // GLM stuff
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    shader.setMat4("transform", trans);

    // All together: V_clip = M_projection * M_view * M_model * V_local
    // create model matrix such that our objects lays on the floor on the x axis
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.f, 0.f));

    // create view matrix that moves the world away from us
    glm::mat4 view = glm::mat4(1.0f);
    // we translate the scene in the reverse direction of where we want to move
//    view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

    // projection matrix
//    glm::mat4 ortho = glm::ortho(0.0f, (float)ARGONAUT_WINDOW_WIDTH, 0.0f, (float)ARGONAUT_WINDOW_HEIGHT, 0.1f, 100.f);
    glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)ARGONAUT_WINDOW_WIDTH / (float)ARGONAUT_WINDOW_HEIGHT, 0.1f, 100.f);
    shader.setMat4("projection", proj);
    shader.setFloat("mixRate", 0.3f);

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Camera stuff
    cameraPos = v3(0.f, 0.f, 3.f);
    // Remember that substracting the camera pos from the camera target gives us
    // a vector that starts at the camera target and goes to the cameraPosition
    cameraTarget = v3(0.f, 0.f, 0.f);
    v3 invCameraDir = glm::normalize(cameraPos - cameraTarget);

    // Get vector that points in the positive x axis direction
    v3 up = v3(0.f, 1.f, 0.f);
    v3 cameraRight = glm::normalize(glm::cross(up, invCameraDir));
    cameraUp = glm::cross(invCameraDir, cameraRight);

    //  Create lookat matrix to translate and rotate the world around the camera
    view = glm::lookAt(cameraPos, cameraTarget, up);
    shader.setMat4("view", view);

    // stuff for walking
    cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    double oldTime = glfwGetTime();
    // render loop
	while (!agWindow.ShouldClose())
	{
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - oldTime;

        oldTime = currentTime;
		// Input handling
		processInput(agWindow.GetMainWindow(), deltaTime);

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state using function

		shader.use();
        texture.UseTexture();
        texture2.UseTexture(GL_TEXTURE1);
        glBindVertexArray(VAO);

        // rotating around crates
//        const float radius = 10.0f;
//        double camX = sin(glfwGetTime()) * radius;
//        double camZ = cos(glfwGetTime()) * radius;
//        view = glm::mat4(1.f);
//        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//        shader.setMat4("view", view);

        // update view for movement
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // transformation matrices
//        model = glm::mat4(1.0f);
        for (int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers and check poll IO events
		agWindow.SwapBuffers();
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	// cleanup glfw resources
	glfwTerminate();

	return 0;
}

std::string readFileToString(std::string const& path) {
	std::ifstream stream(path);
	std::stringstream buffer;
	buffer << stream.rdbuf();
	stream.close();
	return buffer.str();
}

void mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    // TODO fix sudden jump on first entry caused by the lastx being extremely different
    // to the position ur mouse entered the screen from.

    double xoffset = xpos - lastX;
    // reversed since y-coords range from bottom to top otherwise we would have
    // reversed pitch
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += (float)xoffset;
    pitch += (float) yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    // Camera lookaround
    // yaw is the counter-clockwise angle starting from the x axis
    // then use trigonometry to get. Similar for the rest
    // https://learnopengl.com/img/getting-started/camera_yaw.png
    // https://learnopengl.com/img/getting-started/camera_pitch.png
    Argonaut::v3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow* window, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Camera movement
    const float cameraSpeed = 5.f;
    float dtf = (float)deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += (dtf * cameraSpeed) * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= (dtf * cameraSpeed) * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (dtf * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * (dtf * cameraSpeed);
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}