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
    context.window.height = 768;
    context.window.width = 1024;
    context.window.title = "OpenGL";
    context.init();

    /* Create rendering context */
    GLContext window(&context);

    // Set shaders
    unsigned program = window.createProgram("resources/shaders/vertex.glsl",
                                            nullptr,
                                            "resources/shaders/fragment.glsl");
    window.useProgram(program);

    // Load textures
    std::vector<unsigned> textures = {
            window.loadTexture("resources/textures/ascensionLogo.dds"),
            window.loadTexture("resources/textures/ascensionLogo.bmp"),
            window.loadTexture("resources/fonts/font.bmp")
    };

    /* Configure scene */
    GLScene scene;

    // configure scene
    elements::Cube myCube({0, 0, 0}, .2f);
    scene.addElement(myCube);
    elements::Cube myCube2({1, 0, 0}, .2f);
    scene.addElement(myCube2);

    scene.view.mat = glm::lookAt(
            glm::vec3(4, 3, 3),
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    scene.projection.mat = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    scene.clearColor = glm::vec4(.4f, .3f, .2f, .0f);

    scene.uni = {
            .matrix_world = "world",
            .matrix_view = "view",
            .matrix_projection = "projection"
    };

    window.setScene(&scene);


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

//
////    int u_light_position = glGetUniformLocation(shader, "light_position");
//
//    int u_texture_sampler = glGetUniformLocation(shader, "texture_sampler");
//    auto u_directional_light = light::uni::getDirectional(shader, "directional_light");
//    light::uni::u_point u_point_light[2] = {
//            light::uni::getPoint(shader, "point_lights[0]"),
//            light::uni::getPoint(shader, "point_lights[1]")
//    };
//    auto u_point_lights_count = glGetUniformLocation(shader, "point_lights_count");
//
//    int u_camera_position = glGetUniformLocation(shader, "camera_position");
//    int u_specular_intensity = glGetUniformLocation(shader, "specular_intensity");
//    int u_specular_power = glGetUniformLocation(shader, "specular_power");
//
//    int u_draw_ui = glGetUniformLocation(shader, "draw_ui");
//
//    // region load mesh
//    std::vector<util::Vertex> vertices;
//    util::loadOBJ("resources/meshes/spaceship.obj", vertices);
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
//    //endregion
//
//    // load textures
//    GLuint ascensionTexture = util::loadDDS("resources/textures/ascensionLogo.dds");
//    if (!ascensionTexture) {
//        fprintf(stdout, "Couldn't load texture.");
//        return -1;
//    }
//    GLuint ascensionTexture_bmp = util::loadBMP("resources/textures/ascensionLogo.bmp");
//    if (!ascensionTexture_bmp) {
//        fprintf(stdout, "Couldn't load texture.");
//        return -1;
//    }
//    GLuint fontTexture = util::loadBMP("resources/fonts/font.bmp");
//    if (!fontTexture) {
//        fprintf(stdout, "Couldn't load texture.");
//        return -1;
//    }
//
//    // Bind our texture in Texture Unit 0
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, ascensionTexture);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, ascensionTexture_bmp);
//    glActiveTexture(GL_TEXTURE2);
//    glBindTexture(GL_TEXTURE_2D, fontTexture);
//
//    glUniform1i(u_texture_sampler, 0);
//
//    {
//        elements::Cube myCube({.0f, .0f, .0f}, .5f);
//        elements::Cube myCube2({.0f, .0f, -1.0f}, .75f);
//        elements::Square mySquare({-2.5f, -5.f, 0.f}, 5.f);
//
//        // animation
//        float r = .5f, increment = 0.005f;
//
//        float yoff, xoff = yoff = 0.0f;
//        float yrot, xrot = yrot = 0.0f;
//
//        double lastTime = glfwGetTime();
//        int frames = 0;
//
//        auto myText = generateText("Hello, world!", fontTexture);
//
//        do {
//            double currentTime = glfwGetTime();
//            frames++;
//
//            if (currentTime - lastTime >= 1.0) {
//                printf("%fms\n", 1000.f / frames);
//                frames = 0;
//                lastTime = currentTime;
//            }
//
//
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            //region Get input
//            // Get mouse position
//            double xpos, ypos;
//            glfwGetCursorPos(window, &xpos, &ypos);
//
//            // Move forward
//            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//                yoff += 0.1f;
//            }
//            // Move backward
//            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//                yoff -= 0.1f;
//            }
//            // Strafe right
//            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//                xoff -= 0.1f;
//            }
//            // Strafe left
//            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//                xoff += 0.1f;
//            }
//
//            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//                yrot += 0.1f;
//            }
//            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//                yrot -= 0.1f;
//            }
//            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//                xrot += 0.1f;
//            }
//            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//                xrot -= 0.1f;
//            }
//            //endregion
//
//            glUniform1i(u_draw_ui, false);
//
//
//            glm::vec3 cameraPosition(4, 3, 3); // Camera is at (4,3,3), in World Space
//
//            // MVP
//            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//
//            // Camera matrix
//            glm::mat4 view = glm::lookAt(
//                    cameraPosition,
//                    glm::vec3(0, 0, 0), // and looks at the origin
//                    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
//            );
//
//            // model matrix : an identity matrix (model will be at the origin)
//            glm::mat4 model = glm::mat4(1.0f);
//
//            glm::mat4 translated = glm::translate(projection, glm::vec3(.0f, .0f, 0.f));
//            glm::mat4 rotated = glm::rotate(glm::rotate(model, xrot, {1.0f, .0f, .0f}),
//                                            yrot, {0.0f, 1.0f, 0.f});
//
//            glUniformMatrix4fv(u_model, 1, GL_FALSE, &rotated[0][0]);
//            glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
//            glUniformMatrix4fv(u_projection, 1, GL_FALSE, &translated[0][0]);
//
//            glUniform3f(u_camera_position, cameraPosition.x, cameraPosition.y, cameraPosition.z);
//            glUniform1f(u_specular_intensity, 5.0f);
//            glUniform1f(u_specular_power, 32);
//
//            //region Main window
//            float ambientIntensity = .05f;
//            glm::vec3 direction(-1.f, -.5f, .3f);
//
//            light::Directional sun({glm::vec3(1.f, .0f, .0f), ambientIntensity, .2f}, direction);
//            light::Point p1({glm::vec3(1.f), .01f, 1.f}, glm::vec3(1.5f - xoff, yoff + -.5f, .0f), {1.f, .01f, .015f});
//            light::Point p2({glm::vec3(.3f, 1.f, .4f), .1f, 2.f}, glm::vec3(-1.f, 3.f, .0f), {1.f, .01f, .015f});
//
//            light::uni::setDirectional(u_directional_light, sun);
//            glUniform1i(u_point_lights_count, 2);
//
//            light::uni::setPoint(u_point_light[1], p2);
//            light::uni::setPoint(u_point_light[0], p1);
//
//            glUniform1i(u_texture_sampler, 0);
//
//            glBindVertexArray(VAO);
//            glBindBuffer(GL_ARRAY_BUFFER, VBO);
//            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//
//            myCube.render();
//            myCube2.render();
//            mySquare.render();
//
//            glUniform1i(u_draw_ui, true);
////            glDisable(GL_DEPTH_TEST);
//            glUniform1i(u_texture_sampler, 2);
//            myText.render();
//
//            r += increment;
//            //endregion
//
//            glfwSwapBuffers(window);
//            glfwPollEvents();
//
//        } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
//                 glfwWindowShouldClose(window) == 0);
//    }

    return 0;
}
