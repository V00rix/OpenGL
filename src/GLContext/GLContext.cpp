//
// Created by vlado on 24-Jul-18.
//

#include "GLContext.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "../util/util.h"


GLContext::GLContext(const GLWindow *context) : context(context), scene(nullptr) {
    glfwSetInputMode((*context).window.ref, GLFW_STICKY_KEYS, GL_TRUE);
}

unsigned GLContext::loadTexture(const char *filePath) {
    util::FExtension ext = util::getFileExtension(filePath);

    GLuint texture;
    switch (ext) {
        case util::bmp:
            texture = util::loadBMP(filePath);
            textures.push_back(texture);
            return texture;
        case util::dds:
            texture = util::loadDDS(filePath);
            textures.push_back(texture);
            return texture;
        default:
            printf("Unsupported texture format %s", filePath);
            return 0;
    }
}

void GLContext::attachScene(const GLScene *scene) {
    this->scene = scene;
}

void GLContext::render() {
    scene->beforeRender();

    double lastTime = glfwGetTime();
    int frames = 0;

    while (!shouldBreak && glfwWindowShouldClose((*context).window.ref) == 0) {
        if (printFps) {
            double currentTime = glfwGetTime();
            frames++;
            if (currentTime - lastTime >= 1.0) {
                printf("%fms\n", 1000.f / frames);
                frames = 0;
                lastTime = currentTime;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        (*inputHandler).processInput();
        (*scene).render();
        glfwSwapBuffers((*context).window.ref);
        glfwPollEvents();
    }

    scene->afterRender();
}

void GLContext::breakLoop() {
    shouldBreak = true;
}

void GLContext::setInputHandler(const GLInputHandler *handler) {
    inputHandler = handler;
}

const GLInputHandler *GLContext::getInputHandler() const {
    return inputHandler;
}
