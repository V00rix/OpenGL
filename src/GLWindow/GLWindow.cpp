//
// Created by vlado on 24-Jul-18.
//

#include <iostream>
#include <GL/glew.h>
#include "GLWindow.h"


/**
 * OpenGL error handler
 */
static void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar *message, const void *userParam) {
    printf("%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        printf("Aborting...\n");
        exit(3001);
    }
}

GLWindow *GLWindow::initialized = nullptr;

GLWindow::~GLWindow() {
    if (GLWindow::initialized == this) {
        close();
    }
}

GLWindow *GLWindow::init() {
    if (GLWindow::initialized != nullptr) {
        (*GLWindow::initialized).close();
    }
    GLWindow::initialized = this;


    if (!initGLFW())
        return nullptr;

    if (!createWindow() || !initGLEW()) {
        close();
        return nullptr;
    }

    if (debug) {
        std::cout << "Debug output enabled" << std::endl;
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglCallbackFunction, nullptr);
    }

    return this;
}

void GLWindow::close() {
    glfwTerminate();
    GLWindow::initialized = nullptr;
}

bool GLWindow::initGLFW() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, antiAliasing);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forwardCompatibility);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

    return true;
}

bool GLWindow::createWindow() {
    window.ref = glfwCreateWindow(window.width, window.height, window.title, nullptr, nullptr);
    if (window.ref == nullptr) {
        printf("Failed to create GLFW window.\n");
        return false;
    }

    glfwMakeContextCurrent(window.ref);
    glfwSwapInterval(vsync);
    return true;
}

bool GLWindow::initGLEW() {
    glewExperimental = (GLboolean) experimental;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return false;
    }
    return true;
}
