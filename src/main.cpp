// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// other stuff
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "elements/Cube.h"
#include "util/util.h"

#define _DEBUG 1

//region Helpers
/**
 * Main window
 */
static GLFWwindow *window;

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
    glfwSwapInterval(1);
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

/**
 * Compile and load shaders
 * @param vs Path to vertex shader file
 * @param fs Path to fragment shader file
 * @return Program id
 */
static unsigned int loadShaders(const char *vs, const char *fs) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vs, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        printf("Couldn't open shader file: %s.\n", vs);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fs, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vs);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage((unsigned) InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fs);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage((unsigned) InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage((unsigned) InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

/**
 * Error handler function
 * @param source
 * @param type
 * @param id
 * @param severity
 * @param length
 * @param message
 * @param userParam
 */
static void APIENTRY
openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                       const void *userParam) {
    (void) source;
    (void) type;
    (void) id;
    (void) severity;
    (void) length;
    (void) userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}

/**
 * Calculate model-view-projection matrix
 * @return
 */
glm::mat4 calculateMVP() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 view = glm::lookAt(
            glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // model matrix : an identity matrix (model will be at the origin)
    glm::mat4 model = glm::mat4(1.0f);

    return projection * view * model;
}
//endregion

int main() {

    //region Initialization
    if (!initGLFW()) { return -1; }
    if (!createWindow()) { return -1; }
    if (!initGLEW()) { return -1; }

#if _DEBUG
    // Request a debug context.
    std::cout << "Debug output enabled!" << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
#endif

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //endregion

    // shader
    auto shader = loadShaders("resources/shaders/vertex.glsl",
                              "resources/shaders/fragment.glsl"); // this is relative to target binary u_color
    glUseProgram(shader);

    // uniforms
    int MVP_id = glGetUniformLocation(shader, "MVP");
    int scale_id = glGetUniformLocation(shader, "u_scale");
    int translation_id = glGetUniformLocation(shader, "u_translate");
    int texture_sampler_id = glGetUniformLocation(shader, "texture_sampler");

    // MVP
    glm::mat4 MVP = calculateMVP();
    glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);

    // Clear color
    glClearColor(0.176f, 0.313f, 0.325f, 1.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);

    // load textures
    GLuint textureId = util::loadDDS("resources/textures/ascensionLogo.dds");
    if (!textureId) {
        fprintf(stdout, "Couldn't load texture.");
        return -1;
    }


    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(texture_sampler_id, 0);

    {
        util::Vertex vertices[4] = {
                util::Vertex(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f)), // 0
                util::Vertex(glm::vec3(1.f, -1.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f)), // 1
                util::Vertex(glm::vec3(1.f, -1.f, 1.f), glm::vec3(1.f, 0.f, 1.f), glm::vec2(1.f, 1.f)), // 2
                util::Vertex(glm::vec3(0.f, -1.f, 1.f), glm::vec3(1.f, 0.f, 1.f), glm::vec2(0.f, 1.f)), // 3
        };

        unsigned indices[6]{
                0, 1, 2,
                0, 3, 2
        };
//
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint IBO;
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        elements::Cube myCube(glm::vec3(.0f, .0f, .0f), .5f);
        elements::Cube myCube2({.0f, .0f, -1.0f}, .75f);

        // animation
        float r = .5f, increment = 0.005f;

        float yoff, xoff = yoff = 0.0f;


        do {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            // Get mouse position
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            // Move forward
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                yoff += 0.1f;
            }
            // Move backward
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                yoff -= 0.1f;
            }
            // Strafe right
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                xoff += 0.1f;
            }
            // Strafe left
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                xoff -= 0.1f;
            }

            //region Main loop
            glm::mat4 scale = glm::scale(glm::mat4(1.f),
                                         glm::vec3(1.f, 1.f, 1.f));
            glm::mat4 rotate = glm::rotate(scale, (r - .5f), glm::vec3(1.f, 1.f, 1.f));
            glm::mat4 translate = glm::translate(glm::mat4(),
                                                 glm::vec3(-std::sin(r) - 1.f + xpos / 1000 + xoff, ypos / 1000 + yoff,
                                                           -std::cos(r)));

            glUniformMatrix4fv(scale_id, 1, GL_FALSE, &rotate[0][0]);
            glUniformMatrix4fv(translation_id, 1, GL_FALSE, &translate[0][0]);

            myCube.draw();
            myCube2.draw();

//            glBindBuffer(GL_ARRAY_BUFFER, VBO);
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, nullptr);
//            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::color_offset);
//            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, util::Vertex::size, (void *) util::Vertex::uv_offset);
//            glEnableVertexAttribArray(0);
//            glEnableVertexAttribArray(1);
//            glEnableVertexAttribArray(2);
//
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//            glDisableVertexAttribArray(0);
//            glDisableVertexAttribArray(1);
//            glDisableVertexAttribArray(2);

            r += increment;
            //endregion

            glfwSwapBuffers(window);
            glfwPollEvents();

        } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                 glfwWindowShouldClose(window) == 0);
    }

    //region Termination
    glDeleteProgram(shader);
    glfwTerminate();
    //endregion

    return 0;
}
