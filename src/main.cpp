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

int main() {
    /* Create OpenGL window */
    GLWindow context;
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
    GLScene scene;

    // Add elements
    elements::Cube myCube({.0f, .0f, .0f}, .5f);
    elements::Cube myCube2({.0f, .0f, -1.0f}, .75f);
    elements::Square mySquare({-2.5f, -5.f, 0.f}, 5.f);
    elements::Mesh myMesh("resources/meshes/spaceship.obj", {0.f, 0.f, 0.f});
    scene.addElement(myCube);
    scene.addElement(myCube2);
    scene.addElement(mySquare);
    scene.addElement(myMesh);

    // Add lights
    light::Directional sun({glm::vec3(1.f, .0f, .0f), .1f, .2f}, {-1.f, -.5f, .3f});
    light::Point p1({glm::vec3(1.f), .01f, 1.f}, glm::vec3(1.5f, -.5f, .0f), {1.f, .01f, .015f});
    light::Point p2({glm::vec3(.3f, 1.f, .4f), .1f, 2.f}, glm::vec3(-1.f, 3.f, .0f), {1.f, .01f, .015f});
    scene.addLight(sun);
    scene.addLight(p1);
    scene.addLight(p2);

    // Configure matrices
    scene.setView(
            glm::vec3(4, 3, 3),
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0, -1, 0 to look upside-down)
    );
    scene.projection.mat = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Set clear color
//    scene.clearColor = glm::vec4(.4f, .3f, .2f, .0f);

    // Set uniform locations
    scene.uniforms = {
            .matrix_world = "world",
            .matrix_view = "view",
            .matrix_projection = "projection",
            .camera_position = "camera_position",
            .texture_sampler = "texture_sampler",
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

    input.onMouse(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, [&]() {
        printf("Left mouse button is pressed\n");
    });

    window.setInputHandler(&input);

    /* Enter rendering loop */
    window.render();

    // region load mesh
//    std::vector<util::Vertex> vertices;
//    unsigned vertexCount = vertices.size();
//
//    GLuint VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(util::Vertex) * vertexCount, &vertices[0], GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);
//
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //endregion

//
//        auto myText = generateText("Hello, world!", fontTexture);
//
//        do {

//            glBindVertexArray(VAO);
//            glBindBuffer(GL_ARRAY_BUFFER, VBO);
//            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//
//

    return 0;
}
