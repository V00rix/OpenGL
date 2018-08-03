//
// Created by vlado on 24-Jul-18.
//

#include <iostream>
#include <GL/glew.h>
#include "GLWindow.h"


/**
 * OpenGL error handler
 */
static void APIENTRY openglCallbackFunction(GLenum, GLenum, GLuint, GLenum severity, GLsizei,
                                            const GLchar *message, const void *) {
    printf("%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        exit(GL_CRASH_CODE);
    }
}

static void resizeCallback(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
}

GLWindow *GLWindow::initialized = nullptr;

GLWindow::GLWindow() {
    init();
}

GLWindow::GLWindow(bool debug) : debug{debug} {
    init();
}

GLWindow::GLWindow(unsigned width, unsigned height, bool debug) : width{width}, height{height}, debug{debug} {
    init();
}

GLWindow::GLWindow(unsigned width, unsigned height, bool limitCursor, bool debug) : width{width}, height{height},
                                                                                    limitCursor{limitCursor},
                                                                                    debug{debug} {
    init();
}

GLWindow::GLWindow(unsigned width, unsigned height, const char *title, bool debug) : width{width}, height{height},
                                                                                     title{title}, debug{debug} {
    init();
}

GLWindow::~GLWindow() {
    if (GLWindow::initialized == this) {
        cleanGL();
    }
}

void GLWindow::printData() const {
    printf("Printing data for GLWindow (%x):\n", this);
    printf("--- todo ---\n");
}

void GLWindow::cleanGL() const {
    glfwTerminate();
    GLWindow::initialized = nullptr;
}

void GLWindow::initGLFW() const {
    if (!glfwInit()) {
        crashGL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forwardCompatibility);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
}

void GLWindow::createWindow() {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        crashGL("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsync.swapInterval);
}

void GLWindow::initGLEW() const {
    glewExperimental = (GLboolean) experimental;
    if (glewInit() != GLEW_OK) {
        crashGL("Failed to initialize GLEW");
    }
}

void GLWindow::init() {
    if (GLWindow::initialized != nullptr) {
        (*GLWindow::initialized).cleanGL();
    }
    GLWindow::initialized = this;

    initGLFW();
    createWindow();
    initGLEW();

    if (debug) {
        printf("Debug output enabled\n");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(openglCallbackFunction, nullptr);
    }

    glfwSetFramebufferSizeCallback(window, resizeCallback);
}
