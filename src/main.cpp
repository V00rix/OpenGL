// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util/Vector.h"

static GLFWwindow *window;


//region Helpers

/**
 * Initialize GLFW
 * @return
 */
static const bool initGLFW() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

/**
 * Open a window and create its OpenGL context
 * @return
 */
static bool createWindow() {
    window = glfwCreateWindow(1024, 768, "OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

/**
 * Initialize GLEW
 * @return
 */
static bool initGLEW() {
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }
    return true;
}
//endregion

struct Vector3f {
    float x, y, z;
};

int main() {
    //region Initialization
    if (!initGLFW()) { return -1; }
    if (!createWindow()) { return -1; }
    if (!initGLEW()) { return -1; }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //endregion

    std::cout << glGetString(GL_VERSION) << std::endl;

    const float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        //region Main loop

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexArrayAttrib(vertexBuffer, 0);
        //endregion

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    //region Termination
    glfwTerminate();
    //endregion

    return 0;
}
