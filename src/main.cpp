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

    const float positions[] = {
            -1.0f, -1.0f, 0.0f,     // 0
            1.0f, -1.0f, 0.0f,      // 1
            1.0f, 1.0f, 0.0f,       // 2
            -1.0f, 1.0f, 0.0f,       // 3
            1.0f, 1.0f, -1.0f,      // 4
            1.0f, -1.0f, -1.0f       // 5
    };

    const auto red = {1.0f, 0.0f, 0.0f};

    const float colors[] = {
            1.0f, 0.0f, 0.0f,   // 0
            1.0f, 0.0f, 0.0f,   // 1
            1.0f, 0.0f, 0.0f,   // 2
            0.0f, 1.0f, 0.0f,   // 3
            0.0f, 1.0f, 0.0f,   // 4
            0.0f, 1.0f, 0.0f,   // 5
    };


    const unsigned int indices[]{
            0, 1, 2,
            2, 3, 0,
            1, 4, 2,
            1, 5, 4
    };

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    auto shader = loadShaders("shaders/vertex.glsl",
                              "shaders/fragment.glsl"); // this is relative to target binary u_color
    glUseProgram(shader);

    int MVP_id = glGetUniformLocation(shader, "MVP");

    glm::mat4 MVP = calculateMVP();

    int color_id = glGetUniformLocation(shader, "u_color");
    int scale_id = glGetUniformLocation(shader, "u_scale");
    int translation_id = glGetUniformLocation(shader, "u_translate");

    // animation
    float r = .5f, increment = 0.005f;

    glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);

    glClearColor(0.176f, 0.313f, 0.325f, 1.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        //region Main loop

        glUniform4f(color_id, std::sin(r), 1.f, .8f, 1.0f);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f),
                                     glm::vec3(std::sin(r * .3f), std::sin(r * 1.2f), 2.f * std::sin(r)));
        glm::mat4 rotate = glm::rotate(scale, std::cos(r * 4.f), glm::vec3(1.f, 1.0f, .0f));
        glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(-std::sin(r) - 1.f, .0f, -std::cos(r)));

        glUniformMatrix4fv(scale_id, 1, GL_FALSE, &rotate[0][0]);
        glUniformMatrix4fv(translation_id, 1, GL_FALSE, &translate[0][0]);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

        r += increment;
        //endregion

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    //region Termination
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer );
    glDeleteBuffers(1, &indexBuffer);
    glDeleteProgram(shader);
    glfwTerminate();
    //endregion

    return 0;
}
