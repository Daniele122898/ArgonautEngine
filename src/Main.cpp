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
#include "Renderer/Camera.h"
#include "Base/Log.h"
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/PointLight.h"

// globals
const int ARGONAUT_WINDOW_WIDTH = 1280;
const int ARGONAUT_WINDOW_HEIGHT = 720;

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

bool flashlightEnabled = false;

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

	// Building Shader
    Argonaut::Shader shader("src/Renderer/Shaders/Simple/light_vert.glsl",
                  "src/Renderer/Shaders/Simple/light_frag.glsl");
    Argonaut::Shader lampShader("src/Renderer/Shaders/Simple/lamp_vert.glsl",
                            "src/Renderer/Shaders/Simple/lamp_frag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    uint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    uint lampVAO;
    glGenVertexArrays(1,  &lampVAO);
    glBindVertexArray(lampVAO);
    // We again want a cube so we can just bind the same vertex data again
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the atributes
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    shader.use();

    // GLM stuff
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
    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // Camera stuff
    Camera camera = Camera(
            v3(0.f, 0.f, 3.f),
            v3(0.f, 1.f, 0.f),
            -90.f, 0.f, 5.f, 0.1f
            );

    view = camera.CalculateViewMatrix();
    shader.setMat4("view", view);
    shader.setVec3("viewPos", camera.GetPosition());

    // setup material
//    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);
    // set diffuse map
    shader.setInt("material.diffuse", 0); // texture unit 0
    Argonaut::Texture texture("assets/Textures/Simple/container2.png", GL_RGBA);
    texture.LoadTexture();
    // set specular map
    shader.setInt("material.specular", 1);
    Argonaut::Texture specular("assets/Textures/Specular/container2_specular.png", GL_RGBA);
    specular.LoadTexture();

    // set lights
    // SPOT LIGHT
    shader.setVec3("spotLight.position", camera.GetPosition());
    shader.setVec3("spotLight.direction", camera.GetDirection());
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader.setVec3("spotLight.ambient",  0.f, 0.f, 0.f);
    // darken diffuse light a bit
    shader.setVec3("spotLight.diffuse",  0.7f, 0.7f, 0.7f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    // attenuation
    shader.setFloat("spotLight.constant",  1.0f);
    shader.setFloat("spotLight.linear",    0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);

    // DIRECTIONAL LIGHT
    Argonaut::DirectionalLight dirLight(
            {0.05f, 0.05f, 0.05f}, {0.5f, 0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.2f, -1.0f, -0.3f});
    dirLight.UseLight(shader);

    // POINT LIGHTS
    glm::vec3 pointLightDiffuseColors[] = {
            glm::vec3( 0.8f, 0.8f, 0.8f),
            glm::vec3(0.8f, 0.8f, 0.8f),
            glm::vec3( 0.8f, 0.8f, 0.8f),
            glm::vec3( 0.2f, 0.2f, 1.f),
    };
    // point light 1
    Argonaut::PointLight pl1({0.05f, 0.05f, 0.05f}, pointLightDiffuseColors[0],
                             {1.0f, 1.0f, 1.0f}, pointLightPositions[0],
                             1.0f, 0.09f, 0.032f) ;
    pl1.UseLight(shader, 0);
    // point light 2
    Argonaut::PointLight pl2({0.05f, 0.05f, 0.05f}, pointLightDiffuseColors[1],
                             {1.0f, 1.0f, 1.0f}, pointLightPositions[1],
                             1.0f, 0.09f, 0.032f);
    pl2.UseLight(shader, 1);
    // point light 3
    Argonaut::PointLight pl3({0.05f, 0.05f, 0.05f}, pointLightDiffuseColors[2],
                             {1.0f, 1.0f, 1.0f}, pointLightPositions[2],
                             1.0f, 0.09f, 0.032f);
    pl3.UseLight(shader, 2);
    // point light 4
    Argonaut::PointLight pl4({0.05f, 0.05f, 0.05f}, pointLightDiffuseColors[3],
                             {0.2f, 0.2f, 1.0f}, pointLightPositions[3],
                             1.0f, 0.09f, 0.032f);
    pl4.UseLight(shader, 3);

    lampShader.use();
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", proj);
    lampShader.setVec3("lightColor", 1, 1, 1);

    double oldTime = glfwGetTime();
    // render loop
	while (!agWindow.ShouldClose())
	{
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - oldTime;

        oldTime = currentTime;
		// Input handling
		processInput(agWindow.GetMainWindow());

		// Rendering
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state using function

        // update view for movement
        camera.KeyControls(agWindow.GetKeys(), deltaTime);
        camera.MouseControls(agWindow.GetXChange(), agWindow.GetYChange());

        view = camera.CalculateViewMatrix();

        // Point Lights!
        lampShader.use();
        lampShader.setMat4("view", view);
        glBindVertexArray(lampVAO);

        for (int i =0; i<4; ++i) {
            glm::mat4 lampModel = glm::mat4(1.0f);
            lampModel = glm::translate(lampModel, pointLightPositions[i]);
            lampModel = glm::scale(lampModel, glm::vec3(0.2f));
            lampShader.setMat4("model", lampModel);
            lampShader.setVec3("lightColor", pointLightDiffuseColors[i]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        shader.use();
        shader.setMat4("view", view);
        shader.setVec3("viewPos", camera.GetPosition());
        shader.setVec3("spotLight.position", camera.GetPosition());
        shader.setVec3("spotLight.direction", camera.GetDirection());
        // cube
        texture.UseTexture(0);
        specular.UseTexture(1);

        if (flashlightEnabled) {
            shader.setVec3("spotLight.diffuse",  0.7f, 0.7f, 0.7f);
            shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        } else {
            shader.setVec3("spotLight.diffuse",  0.f, 0.f, 0.f);
            shader.setVec3("spotLight.specular", 0.f, 0.f, 0.f);
        }

        glBindVertexArray(VAO);

        for(uint i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            glm::mat4 normal = glm::transpose(glm::inverse(model));
            shader.setMat3("normalM", glm::mat3(normal));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers and check poll IO events
		agWindow.SwapBuffers();
		glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

	// cleanup glfw resources
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        flashlightEnabled = !flashlightEnabled;
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}