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
//region Helpers
static elements::Square generateText(const char *string, GLuint texture) {
    const glm::vec2 uv[4] = {
            glm::vec2(.0f, .0f),
            glm::vec2(1.0f / 16.f, .0f),
            glm::vec2(1.0f / 16.f, 1.0f / 16.f),
            glm::vec2(.0f, 1.0f / 16.f),
    };

    return elements::Square(glm::vec3(-.5f, -.5f, .0f), .2f, uv);
}
//endregion

static void printArray(int * arr, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d: %d\n", i, arr[i]);
    }
}

int main() {
    GLWindow context;
    GLScene scene;

    context = GLWindow();
    /* Create OpenGL window */
    context.debug = true;
    context.window.height = 768;
    context.window.width = 1024;
    context.window.title = "OpenGL";
    context.init();

    /* Create rendering context */
    GLContext window(&context);
    window.printFps = true;

    // Set shaders
    unsigned program = window.createProgram("resources/shaders/vertex.glsl",
                                            nullptr,
                                            "resources/shaders/fragment.glsl");
    window.useProgram(program);

    // Load textures
    std::vector<unsigned> textures = {
            window.loadTexture("resources/textures/ascensionLogo.bmp"),
            window.loadTexture("resources/fonts/font.bmp"),
            window.loadTexture("resources/textures/ascensionLogo.dds"),
    };

    /* Configure scene */
    scene.grid = GLScene::Grid(10.f, 10);
    scene.renderGrid = true;

    // Add elements
    elements::Cube myCube({.0f, .0f, .0f}, .5f);
    elements::Cube myCube2({.0f, .0f, -1.0f}, .75f);
    elements::Square mySquare({-2.5f, -5.f, 0.f}, 5.f);
    elements::Mesh myMesh("resources/meshes/spaceship.obj", {0.f, 0.f, 0.f});
    elements::Mesh myLightMesh("resources/meshes/sphere.obj", {0.f, 0.f, 0.f});
    myLightMesh.scale({.25f, .25f, .25f});
    scene.addElement(myCube);
    scene.addElement(myCube2);
    scene.addElement(mySquare);
    scene.addElement(myMesh);

    // Add lights
    scene.setLightMesh(myLightMesh);

    light::Directional sun({glm::vec3(1.f, .0f, .0f), .1f, .2f}, {-1.f, -.5f, .3f});
    light::Point p1({glm::vec3(1.f), .01f, 1.f}, glm::vec3(1.5f, -.5f, 1.0f), {1.f, .01f, .015f});
    light::Point p2({glm::vec3(.3f, 1.f, .4f), .1f, 2.f}, glm::vec3(-1.5f, -.5f, 1.0f), {1.f, .01f, .015f});
    scene.addLight(sun);
    scene.addLight(p1);
    scene.addLight(p2);

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
                    .directional_count = "directional_lights_count",
                    .directional = "directional_lights",
                    .point_count = "point_lights_count",
                    .point = "point_lights",
            }
    };

    // Complete scene configuration
    window.attachScene(&scene);

    /* Configure input */
    GLInputHandler input(context.window.ref);
    input.onKey(GLFW_KEY_ESCAPE, GLFW_PRESS, [&]() {
        window.breakLoop();
    });

    input.onKey(GLFW_KEY_P, GLFW_PRESS, [&]() {
        printf("P key is pressed\n");
    });

    //region Rotation
    input.onKey(GLFW_KEY_U, GLFW_PRESS, [&]() {
        myMesh.rotate(.1f, {0.0f, 1.0f, 0.0f});
    });

    input.onKey(GLFW_KEY_Y, GLFW_PRESS, [&]() {
        myMesh.rotate(-.1f, {0.0f, 1.0f, 0.0f});
    });
    //endregion

    //region Translation
    input.onKey(GLFW_KEY_V, GLFW_PRESS, [&]() {
        myMesh.translate({0.0f, 0.f, 0.1f});
    });
    input.onKey(GLFW_KEY_B, GLFW_PRESS, [&]() {
        myMesh.translate({0.0f, 0.f, -0.1f});
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

    input.onMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, [&]() {
        printf("Left mouse button is pressed\n");
    });

    window.setInputHandler(&input);

    /* Enter rendering loop */
    window.render();

    return 0;
}
