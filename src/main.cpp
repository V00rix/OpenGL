// OpenGL
#include <GL/glew.h>
// other stuff
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "elements/Cube/Cube.h"
#include "util/util.h"
#include "elements/Square/Square.h"
#include "glfw3.h"
#include "light/light.h"
#include "GLWindow/GLWindow.h"
#include "GLContext/GLContext.h"
#include "GLContext/GLScene/GLScene.h"
#include "GLContext/GLInputHandler/GLInputHandler.h"
#include "elements/Mesh/Mesh.h"
#include "glm/gtc/constants.hpp"
#include "Program/Program.h"
#include "Texture/Texture.h"
#include "elements/Skybox/Skybox.h"

//region Helpers
static elements::Square generateText(const char *string, GLuint texture) {
    const glm::vec2 uv[4] = {
            glm::vec2(.0f, .0f),
            glm::vec2(1.0f / 16.f, .0f),
            glm::vec2(1.0f / 16.f, 1.0f / 16.f),
            glm::vec2(.0f, 1.0f / 16.f),
    };

    return elements::Square(.2f, uv);
}
//endregion

static void printArray(int *arr, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d: %d\n", i, arr[i]);
    }
}

int main() {
    GLWindow window;
    GLScene scene;

    /* Create OpenGL window */
    window = GLWindow();
    window.debug = true;
    window.limitCursor = true;
    window.window.height = 1468;
    window.window.width = 2024;
    window.window.title = "OpenGL";
    window.init();

    /* Create rendering context */
    GLContext context(&window);
    context.printFps = true;

    // Set shaders
    scene.program.set("resources/shaders/vertex.glsl",
                      "resources/shaders/fragment.glsl");
    scene.stencilProgram.set("resources/shaders/vertex.glsl",
                             "resources/shaders/stencil.glsl");
    scene.skyboxProgram.set("resources/shaders/skybox/vertex.glsl",
                            "resources/shaders/skybox/fragment.glsl");

    scene.useProgram(&scene.program);

    // Load textures
    std::vector<Texture> textures = {
            Texture("resources/textures/ascensionLogo.bmp"),
            Texture("resources/fonts/font.bmp"),
            Texture("resources/textures/ascensionLogo.dds"),
            Texture("resources/textures/simple.bmp"),
            Texture("resources/textures/sampleTexture.dds"),
    };

    /* Configure scene */
    scene.grid = GLScene::Grid(10.f, 10);
    scene.renderGrid = true;
    scene.stencilTest = true;
    // Add elements
    elements::Cube myCube(.5f);
    elements::Cube myCube2(.75f);
    elements::Square mySquare(5.f);
    elements::Skybox mySkybox((const char *const[6]) {
            "resources/textures/skyboxes/mp_jasper/front.bmp",
            "resources/textures/skyboxes/mp_jasper/back.bmp",
            "resources/textures/skyboxes/mp_jasper/down.bmp",
            "resources/textures/skyboxes/mp_jasper/up.bmp",
            "resources/textures/skyboxes/mp_jasper/right.bmp",
            "resources/textures/skyboxes/mp_jasper/left.bmp"
    });
//    mySquare.rotate(45.f, {1.f, .0f, .0f});
    mySquare.rotate(-glm::half_pi<float>(), {1.0f, 0.0f, 0.0f});
    mySquare.translate({-2.5f, -1.f, 2.5f});
    myCube.translate({1.f, 0.f, 0.f});
//    mySquare.translate({-2.5f, -5.f, 0.f});
    elements::Mesh myMesh("resources/meshes/spaceship.obj");
    myMesh.translate({0.f, .5f, .0f});
    elements::Mesh myLightMesh("resources/meshes/sphere.obj");
    myLightMesh.scale({.25f, .25f, .25f});
    scene.addElement(myCube);
//    scene.addElement(myCube2);
    scene.addElement(mySquare);
    scene.addElement(myMesh);

    scene.skybox = &mySkybox;

            // Add lights
    scene.setLightMesh(myLightMesh);

    light::Directional sun({glm::vec3(1.f, .0f, .0f), .1f, .2f}, {-1.f, -.5f, .3f});
    light::Point p1({glm::vec3(2.f), .01f, .3f}, glm::vec3(1.5f, 2.f, 0.0f), {1.f, .01f, .015f});
    light::Point p2({glm::vec3(.3f, 1.f, .4f), .3f, 1.f}, glm::vec3(-1.5f, 1.f, 0.0f), {1.f, .01f, .015f});
    light::Spot spotLight(light::Point({glm::vec3(1.f, 0.f, .4f), .05f, 1.3f},
                                       glm::vec3(2.f, 0.5f, 0.0f),
                                       {1.f, .01f, .015f}),
                          {-1.f, -1.f, 0.f},
                          0.8f);
//    scene.addLight(sun);
    scene.addLight(p1);
    scene.addLight(p2);
    scene.addSpotlight(spotLight);

    // Configure matrices
    scene.setView(
            glm::vec3(4, 3, 3), // position in (4, 3, 3)
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0, -1, 0 to look upside-down)
    );
    scene.projection.mat = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Set clear color
    scene.clearColor = glm::vec4(.15f, .16f, .18f, .0f);

    // Set uniform locations
    scene.uniforms = {
            .matrix_world = "world",
            .matrix_view = "view",
            .matrix_projection = "projection",
            .camera_position = "camera_position",
            .texture_sampler = "texture_sampler",
            .grid_enabled = "grid_enabled",
            .light_mesh = "light_mesh",
            .specular_intensity = "specular_intensity", // todo: belongs to objects and materials
            .specular_power = "specular_power", // todo: belongs to objects and materials
            .lights = {
                    .directional = "directional_lights",
                    .directional_count = "directional_lights_count",
                    .directional_index = "directional_index",
                    .point = "point_lights",
                    .point_count = "point_lights_count",
                    .point_index = "point_index",
                    .spot = "spot_lights",
                    .spot_count = "spot_lights_count",
                    .spot_index = "spot_index",
            }
    };

    float cameraSpeed = 0.05f;
    float deltaTime, lastTime = deltaTime = 0.f;
    scene.onRender([&]() {
        auto currentTime = (float) glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        cameraSpeed = 2.5f * deltaTime;
    });

    // Complete scene configuration
    context.attachScene(&scene);

    /* Configure input */
    GLInputHandler input(window.window.ref);

    //region Exit button
    input.onKey(GLFW_KEY_ESCAPE, GLFW_PRESS, [&]() {
        context.breakLoop();
    });
    //endregion

    //region Spotlight
    bool changeSpotlight = true;
    input.onKey(GLFW_KEY_L, GLFW_PRESS, [&]() {
        if (changeSpotlight) {
            scene.spotlight = !scene.spotlight;
            changeSpotlight = false;
        }
    });
    input.onKey(GLFW_KEY_L, GLFW_RELEASE, [&]() {
        changeSpotlight = true;
    });
    //endregion

    //region Camera movement
    glm::vec3 cameraPos(1.f, 3.f, 5.5f);
    glm::vec3 cameraFront(0.f, 0.f, -1.f);
    glm::vec3 cameraUp(0.f, 1.f, 0.f);
    scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);

    input.onKey(GLFW_KEY_W, GLFW_PRESS, [&]() {
        cameraPos += cameraSpeed * (cameraFront);
        scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);
    });
    input.onKey(GLFW_KEY_S, GLFW_PRESS, [&]() {
        cameraPos -= cameraSpeed * (cameraFront);
        scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);
    });
    input.onKey(GLFW_KEY_A, GLFW_PRESS, [&]() {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);
    });
    input.onKey(GLFW_KEY_D, GLFW_PRESS, [&]() {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);
    });


    float sensitivity = 0.05f;


    float lastX = window.window.width / 2.f;
    float lastY = window.window.height / 2.f;
    float yaw = -105.f, pitch = -25.0f;

    bool firstMouse = true;
    input.onCursor([&](int xPos, int yPos) {
        if (firstMouse) {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        float xOffset = xPos - lastX;
        float yOffset = lastY - yPos;
        lastX = xPos;
        lastY = yPos;

        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = (float) (cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front.y = (float) sin(glm::radians(pitch));
        front.z = (float) (sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        cameraFront = glm::normalize(front);

        scene.setView(cameraPos, cameraPos + cameraFront, cameraUp);
    });
    //endregion

    //region Texture change
    int texture = 0;
    bool shouldChange = true;
    input.onKey(GLFW_KEY_T, GLFW_RELEASE, [&]() {
        shouldChange = true;
    });
    input.onKey(GLFW_KEY_T, GLFW_PRESS, [&]() {
        if (shouldChange) {
            textures[++texture % textures.size()].use();
            shouldChange = false;
        }
    });
    //endregion

    //region Program change
    int prog = 0;
    bool progChange = true;
    input.onKey(GLFW_KEY_P, GLFW_RELEASE, [&]() {
        progChange = true;
    });
    input.onKey(GLFW_KEY_P, GLFW_PRESS, [&]() {
        if (progChange) {
            scene.useProgram(prog++ % 2 == 0 ? &scene.stencilProgram : &scene.program);
            progChange = false;
        }
    });
    //endregion

    //region Rotation
    input.onKey(GLFW_KEY_U, GLFW_PRESS, [&]() {
        mySquare.rotate(.1f, {1.0f, 0.0f, 0.0f});
    });

    input.onKey(GLFW_KEY_Y, GLFW_PRESS, [&]() {
        mySquare.rotate(-.1f, {1.0f, 0.0f, 0.0f});
    });
    //endregion

    //region Translation
    input.onKey(GLFW_KEY_V, GLFW_PRESS, [&]() {
        mySquare.translate({0.0f, 0.f, 0.1f});
    });
    input.onKey(GLFW_KEY_B, GLFW_PRESS, [&]() {
        mySquare.translate({0.0f, 0.f, -0.1f});
    });

    input.onKey(GLFW_KEY_Q, GLFW_PRESS, [&]() {
        mySquare.translate({0.1f, 0.f, 0.f});
    });
    input.onKey(GLFW_KEY_E, GLFW_PRESS, [&]() {
        mySquare.translate({-0.1f, 0.f, 0.f});
    });
    //endregion

    //region Scale
    input.onKey(GLFW_KEY_Z, GLFW_PRESS, [&]() {
        myMesh.scale({1.1f, 1.1f, 1.1f});
    });

    input.onKey(GLFW_KEY_X, GLFW_PRESS, [&]() {
        myMesh.scale({0.9f, 0.9f, 0.9f});
    });
    //endregion

    context.setInputHandler(&input);

    /* Enter rendering loop */
    context.render();

    return 0;
}
