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
#include <cstdarg>
#include "GLModel/Mesh/Mesh.h"
#include "Texture/Texture.h"
#include "GLModel/Material/Material.h"
#include "GLModel/GLModel.h"
#include "GLWindow/GLWindow.h"
#include "meshes/Grid/Grid.h"
#include "meshes/Cube/Cube.h"
#include "Program/Program.h"
#include "Camera/Camera.h"
#include "util/uniforms.h"
#include "UniBlock/UniBlock.h"

static GLFWwindow *window;

static void processCamera(Camera *, double, double, double);

static void updateTime(double *delta);

static void glSettings();

static const int height{1468};
static double lastFrame{0.f};
static const int width{2024};

int main() {
    GLWindow w(width, height, true, true);
    window = w.window;

    glSettings();

    //region Compile and load programs
    Program program{"resources/shaders/vertex.glsl",
                    "resources/shaders/fragment.glsl",
                    "resources/shaders/geometry.glsl"},
            stencil{"resources/shaders/vertex.glsl",
                    "resources/shaders/stencil/fragment.glsl"},
            skybox{"resources/shaders/skybox/vertex.glsl",
                   "resources/shaders/skybox/fragment.glsl"};
    //endregion

    //region Block uniforms
    UniBlock matrices{UNI_BLOCK_MATRICES, 3 * sizeof(glm::mat4), 1, program};
    //endregion

    //region Load textures
    Texture textures[] {
            Texture("resources/fonts/font.bmp"),
            Texture("resources/textures/simple.bmp"),
            Texture("resources/textures/sampleTexture.dds"),
            Texture("resources/textures/ascensionLogo.bmp"),
            Texture("resources/textures/ascensionLogo.dds"),
    };
    //endregion

    //region set up models
    Grid grid(10.f, 10);
    Cube cube(.5f);
    cube.transform(glm::translate(glm::vec3{-.25f}));
    GLModel cubeModel(cube, textures[4]);
    //endregion

    //region set up camera
    Camera camera{{2.f, 2.f, -1.f},
                  {0.f, 0.f, 0.f}};
    camera.lastX = width / 2.f;
    camera.lastY = height / 2.f;
    // endregion

    //region Lights
    light::Directional sun({glm::vec3(1.f, 1.0f, 1.0f), .01f, .2f}, {0.f, -1.0f, 0.f});
    //endregion

    double x{0}, y{0}, deltaTime{0};

    //region main loop
    while (!glfwWindowShouldClose(window)) {
        // Widow closing
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // update cursor position
        glfwGetCursorPos(window, &x, &y);

        // update time variables
        updateTime(&deltaTime);

        // clear
        glClearColor(.3f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // set program
        program.use();

        // set camera
        processCamera(&camera, x, y, deltaTime);
        camera.set(matrices);

        // render
        cube.render();

        // end
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //endregion

    // cleanup
    cubeModel.cleanGL();

//    /* Configure scene */
//    scene.grid = GLScene::Grid(10.f, 10);
////    scene.renderGrid = true;
//    scene.stencilTest = true;
//    // Add objects
//    objects::Cube myCube(.5f);
//    objects::Cube myCube2(.75f);
//    objects::Square mySquare(5.f);
//    objects::Skybox mySkybox((const char *const[6]) {
//            "resources/textures/skyboxes/mp_jasper/front.bmp",
//            "resources/textures/skyboxes/mp_jasper/back.bmp",
//            "resources/textures/skyboxes/mp_jasper/down.bmp",
//            "resources/textures/skyboxes/mp_jasper/up.bmp",
//            "resources/textures/skyboxes/mp_jasper/right.bmp",
//            "resources/textures/skyboxes/mp_jasper/left.bmp"
//    });
////    mySquare.rotate(45.f, {1.f, .0f, .0f});
//    mySquare.rotate(-glm::half_pi<float>(), {1.0f, 0.0f, 0.0f});
//    mySquare.translate({-2.5f, -1.f, 2.5f});
//    myCube.translate({1.f, 0.f, 0.f});
////    mySquare.translate({-2.5f, -5.f, 0.f});
////    elements::MeshOld myMesh("resources/meshes/spaceship.obj");
////    myMesh.translate({0.f, .5f, .0f});
////    elements::MeshOld myLightMesh("resources/meshes/sphere.obj");
////    myLightMesh.scale({.25f, .25f, .25f});
//    scene.addElement(myCube);
////    scene.addElement(myCube2);
//    scene.addElement(mySquare);
////    scene.addElement(myMesh);
//
//    scene.skybox = &mySkybox;
//
//            // Add lights
////    scene.setLightMesh(myLightMesh);
//
//    light::Directional sun({glm::vec3(1.f, 1.0f, 1.0f), .01f, .2f}, {0.f, -1.0f, 0.f});
//    light::Point p1({glm::vec3(2.f), .01f, .3f}, glm::vec3(1.5f, 2.f, 0.0f), {1.f, .01f, .015f});
//    light::Point p2({glm::vec3(.3f, 1.f, .4f), .03f, .6f}, glm::vec3(-1.5f, 1.f, 0.0f), {1.f, .01f, .015f});
//    light::Spot spotLight(light::Point({glm::vec3(1.f, 0.f, .4f), .05f, 1.3f},
//                                       glm::vec3(2.f, 0.5f, 0.0f),
//                                       {1.f, .01f, .015f}),
//                          {-1.f, -1.f, 0.f},
//                          0.8f);
//    scene.addLight(sun);
//    scene.addLight(p1);
//    scene.addLight(p2);
//    scene.addSpotlight(spotLight);
//
    return 0;
}

void processCamera(Camera *camera, double x, double y, double delta) {
    // Move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->move(CameraMovement::FORWARD, (float) delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->move(CameraMovement::BACKWARD, (float) delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move(CameraMovement::LEFT, (float) delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->move(CameraMovement::RIGHT, (float) delta);
    }
    // Rotate camera
    camera->rotate((float) x, (float) y);
}

void updateTime(double *delta) {
    double currentFrame = glfwGetTime();
    *delta = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void glSettings() {
    // cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // face culling
    glEnable(GL_CULL_FACE);

    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
}
